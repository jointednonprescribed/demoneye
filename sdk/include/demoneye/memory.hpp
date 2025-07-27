
#ifndef __DEMONEYE__setup_MEMORY_H
#define __DEMONEYE__setup_MEMORY_H 1

#include "setup.hpp"



namespace demoneye
{
	/* Memory/Thread Owner type */
	typedef struct owner_t
	{
		pid_t process;
		gid_t group;

		constexpr owner_t():
			process(NULL_PID), group(NULL_GID)
		{}
		owner_t(const owner_t&);
		owner_t(pid_t process, gid_t group);

		operator pid_t() const;
		operator gid_t() const;
		operator pair<pid_t, gid_t>() const;
		operator pair<gid_t, pid_t>() const;
		
		pid_t Process() const;
		pid_t Group() const;
	} oid_t;
	constexpr oid_t NULL_OID;
	owner_t GetCurrentOID();


	/* Memory Block Header type */
	struct MemBlockHeader
	{
		int     refc, flags;
		owner_t owner;
		size_t  size;
	};

	/* System-managed Heap type */
	class SystemHeap: public iTerminatable
	{
	public:
		/* Heap Handle/Identifier type */
	#if DE_PLATFORM == WINDOWS
		typedef HANDLE handle_t;
		static constexpr handle_t NULL_HANDLE =
		#if DE_PLATFORM32 == true
			0xffffffff;
		#else
			0xffffffffffffffff;
		#endif
	#elif DE_PLATFORM_UNIXLIKE == true
		typedef int handle_t;
		static constexpr handle_t NULL_HANDLE = 0;
	#endif

		/* Memory Block ID type */
		typedef size_t blockid_t;

		/* Memory Block Destructor type */
		typedef function<int, MemBlockHeader*> destructor_t;

		enum Flags //                                 pub: grp: ownr:
		{ //                                       Z A   rw   rw    rw
			null_flags       = 0,

			owner_write      = 01, //     0000 00  0 000 00   00    01
			owner_read       = 02, //     0000 00  0 000 00   00    10
			owner_rw         = 03, //     0000 00  0 000 00   00    11
			owner_perms_bshft = 0,

			group_write      = 04, //     0000 00  0 000 00   01    00
			group_read       = 010, //    0000 00  0 000 00   10    00
			group_rw         = 014, //    0000 00  0 000 00   11    00
			group_perms_bshft = 2,

			public_write     = 020, //    0000 00  0 000 01   00    00
			public_read      = 040, //    0000 00  0 000 10   00    00
			public_rw        = 060, //    0000 00  0 000 11   00    00
			public_perms_bshft = 4,

			owner_auth       = 0100, //   0000 00  0 001 00   00    00
			group_auth       = 0200, //   0000 00  0 010 00   00    00
			public_auth      = 0400, //   0000 00  0 100 00   00    00
			auth_bshft       = 6,

			manual_zero_init = 040, //    0000 00  1 000 00   00    00
			manual_zero_init_bshft = 9,

			/* Defaults */
			public_default_w   = public_read | public_write | group_read | group_write | owner_read | owner_write | owner_auth,
			public_default     = public_read | group_read | owner_read | owner_write | owner_auth,
			group_default_w    = group_read | group_write | owner_write | owner_read | owner_auth,
			group_default      = group_read | owner_write | owner_read | owner_auth,
			private_default    = owner_read | owner_write | owner_auth,
			private_default_ro = owner_read | owner_auth,

			flags_default = public_default,

			/* Aliases */
			read  = public_read | group_read | owner_read,
			write = public_write | group_write | owner_write,
		};

	protected:
		handle_t heap;
		void   **blocklist;
		size_t   blklst_len, blklst_cap;

	public:
		static constexpr size_t DEFAULT_BLOCKLIST_SIZE = 20;

		static handle_t    GetCurrentHandle();
		static SystemHeap& GetCurrent();

		static SystemHeap Current();

		SystemHeap();
		SystemHeap(const SystemHeap&);

		SystemHeap(handle_t);

		~SystemHeap();

		handle_t Handle() const;

		struct BlockRequestTicket
		{
			size_t       total_size;
			int          flags;
			destructor_t destructor;

			BlockRequestTicket();
			BlockRequestTicket(const BlockRequestTicket&);
			BlockRequestTicket(size_t total_size, int flags, destructor_t destructor=nullptr);
		};
		virtual blockid_t NewBlock(size_t total_size, int flags, destructor_t destructor=nullptr);
		virtual blockid_t NewBlock(BlockRequestTicket);
		virtual blockid_t NewAdjacentBlocks(size_t n, const BlockRequestTicket *tickets);
		template <size_t _N>
		blockid_t NewAdjacentBlocks(const BlockRequestTicket tickets[_N])
		{
			return this->NewAdjacentBlocks(_N, tickets);
		}

		virtual int  DropBlock(blockid_t block);
		virtual int  DropBlock(blockid_t block) const;

		virtual int  BorrowBlock(blockid_t block) const;
		virtual bool SetBlockOwnership(blockid_t block, owner_t new_owner);
		virtual int  SetBlockFlags(blockid_t block, int flags);
		virtual int  AddBlockFlags(blockid_t block, int flags);
		virtual int  RemoveBlockFlags(blockid_t block, int flag_mask=-1);

		virtual void*                 GetBlockAddress(blockid_t block);
		virtual const void*           GetBlockConstAddress(blockid_t block) const;
		virtual MemBlockHeader&       GetBlockHeader(blockid_t block);
		virtual const MemBlockHeader& GetBlockConstHeader(blockid_t block)  const;

		virtual size_t                GetBlockSize(blockid_t block)         const;
		virtual int                   GetBlockFlags(blockid_t block)        const;
		virtual const owner_t&        GetBlockOwner(blockid_t block)        const;
		virtual const pid_t&          GetBlockOwnerProcess(blockid_t block) const;
		virtual const gid_t&          GetBlockOwnerGroup(blockid_t block)   const;
	};


	template <typename _Original, typename _ReTyp>
	struct is_reinterpretable
	{
		static constexpr bool value =
			// Reinterpretable as _ReTyp if one of the following is true,
			// (Let "Type 1" be _Original, the original type of the memref
			// object, and "Type 2" be the type to reinterpret the reference
			// or some part of it's memory to):
			//   a) Type 1 and Type 2 are exactly the same.
			//   b) Type 1 is char (byte-by-byte data), and Type 2 is any
			//      type other than void (non-void memory can be reinterpreted
			//      to a byte-by-byte context, but not the inverse).
			//   c) Type 1 is convertible to Type 2 AND Type 2 is convertible
			//      back to Type 1 (types are mutually convertible).
			std::is_same_v<_Original, _ReTyp> ||
			(std::is_same_v<_Original, char> && !std::is_void_v<_ReTyp>) ||
			(std::is_convertible_v<_Original, _ReTyp> && std::is_convertible_v<_ReTyp, _Original>);
	};
	template <typename _Original, typename _ReTyp>
	constexpr bool is_reinterpretable_v = is_reinterpretable<_Original, _ReTyp>::value;

	template <typename _T>
	class memref: public iIterable<_T>
	{
	public:
		using SystemHeap::blockid_t, SystemHeap::destructor_t;

		typedef       _T *ptr_t;
		typedef const _T *const_ptr_t;

	private:
		ownerid_t   refowner;
		SystemHeap *heap;
		blockid_t   id;
		bool        is_block_owner;

	public:
		typedef supplier<pair<SystemHeap*, SystemHeap::blockid_t>> allocator_t;

		memref() noexcept
		{
			refowner       = GetCurrentOID();
			is_block_owner = false;
			heap           = nullptr;
			id             = 0;
		}

		memref(const memref<_T> &rhv) noexcept:
			refowner(GetCurrentOID())
		{
			if (rhv.heap == nullptr) {
				is_block_owner = false;
				heap           = nullptr;
				id             = 0;
				return;
			} else {
				if (refowner.Process() == rhv.refowner.Process() && rhv.is_block_owner)
					is_block_owner = true;
				else
					is_block_owner = false;

				heap = rhv.heap;
				id   = rhv.id;
				return;
			}
		}

		memref(size_t preallocate_length, SystemHeap *heap_ref=&SystemHeap::GetCurrent()) noexcept
		{
			if (heap_ref == nullptr)
				heap_ref = &SystemHeap::GetCurrent();
			refowner       = GetCurrentOID();
			heap           = heap_ref;
			try {
				id             = heap_ref->NewBlock(preallocate_length);
				is_block_owner = true;
			} catch (std::exception e) {
				id             = 0;
				is_block_owner = false;
				heap           = nullptr;
				logerr(e);
			}
		}

		~memref() noexcept
		{
			this->Terminate();
		}

		static memref<_T> New(SystemHeap *heap, size_t amount_of_items, int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			if (amount_of_items < 1) {
				throw logerr(argument_error(
					"Cannot create 0 items inside of a "
					"memory block, argument #2 must be >0."
				));
				return memref<_T>();
			} else if (heap == nullptr)
				heap = &SystemHeap::GetCurrent();

			return memref<_T>(
				heap,
				heap->NewBlock(amount_of_items, flags, destructor),
			)
		}
		static memref<_T> New(size_t amount_of_items, int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			return New(&SystemHeap::GetCurrent(), amount_of_items, flags, destructor);
		}
		static memref<_T> New(SystemHeap *heap, int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			return New(heap, 1, flags, destructor);
		}
		static memref<_T> New(int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			return New(&SystemHeap::GetCurrent(), 1, flags, destructor);
		}
		static memref<_T> New(const _T &value, SystemHeap *heap=nullptr, int flags=SystemHeap::flags_default, destructor_t destructor=nullptr) noexcept(false)
		{
			memref<_T> ref = New(heap, 1, flags, destructor);
			*ref = value;
			return ref;
		}
		static memref<_T> New(const _T *values, size_t amount_of_items, SystemHeap *heap=nullptr, int flags=SystemHeap::flags_default, destructor_t destructor=nullptr) noexcept(false)
		{
			memref<_T> ref = New(heap, amount_of_items, flags, destructor);

			for (size_t i = 0; i < amount_of_items; i++)
				ref.blk[i] = values[i];

			return ref;
		}
		static memref<_T> New(allocator_t allocator)
		{
			pair<SystemHeap*, SystemHeap::blockid_t> idpair = allocator();
			return memref<_T>(idpair.former, idpair.latter, 1, destructor);
		}
		static memref<_T> New(const _T &value, allocator_t allocator) noexcept(false)
		{
			const pair<SystemHeap*, blockid_t> blkid = heap->NewBlock(heap, amount_of_items, flags, destructor);

			if (heap != nullptr)
				Drop();

			heap = idpair.former;
			id   = idpair.latter;

			_T *addr = heap->GetBlockAddress(id);
			*addr = value;
		}
		static memref<_T> New(size_t n, const _T *values, allocator_t allocator) noexcept(false)
		{ // FIX MEMREF OWNERSHIP TO PASS THROUGH C++ COPY PROCEDURES
			const pair<SystemHeap*, blockid_t> blkid = heap->NewBlock(heap, amount_of_items, flags, destructor);
			
			memref<_T> ref(blkid.former, blkid.latter);

			_T *addr = ref.Address();

			for (size_t i = 0; i < amount_of_items; i++)
				addr[i] = values[i];
		}

		void Alloc(SystemHeap *heap_ref, size_t amount_of_items, int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			if (heap_ref == nullptr)
				heap_ref = &SystemHeap::GetCurrent();
			else if (amount_of_items < 1) {
				throw logerr(argument_error(
					"Cannot create 0 items inside of a "
					"memory block, argument #2 must be >0."
				));
				return;
			}

			const blockid_t blkid = heap_ref->NewBlock(amount_of_items * sizeof(_T), flags, destructor);

			if (heap != nullptr)
				Drop();
			
			heap = heap_ref;
			id   = blkid;
		}
		void Alloc(size_t amount_of_items, int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			return New(&SystemHeap::GetCurrent(), amount_of_items, flags, destructor);
		}
		void Alloc(SystemHeap *heap, int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			return New(heap, 1, flags, destructor);
		}
		void Alloc(int flags=SystemHeap::flags_default, SystemHeap::destructor_t destructor=nullptr) noexcept(false)
		{
			return New(&SystemHeap::GetCurrent(), 1, flags, destructor);
		}
		void Alloc(const _T &value, SystemHeap *heap_ref=nullptr, int flags=SystemHeap::flags_default, destructor_t destructor=nullptr) noexcept(false)
		{
			if (heap_ref == nullptr)
				heap_ref = &SystemHeap::GetCurrent();

			const blockid_t blkid = heap->NewBlock(heap, amount_of_items, flags, destructor);

			if (heap != nullptr)
				Drop();

			id   = blkid;
			heap = heap_ref;

			_T *addr = Address();

			*addr = value;
		}
		void Alloc(const _T *values, size_t amount_of_items, SystemHeap *heap_ref=nullptr, int flags=SystemHeap::flags_default, destructor_t destructor=nullptr) noexcept(false)
		{
			if (heap_ref == nullptr)
				heap_ref = &SystemHeap::GetCurrent();

			const blockid_t blkid = heap->NewBlock(heap, amount_of_items, flags, destructor);

			if (heap != nullptr)
				Drop();

			id   = blkid;
			heap = heap_ref;

			_T *addr = Address();

			for (size_t i = 0; i < amount_of_items; i++)
				addr[i] = values[i];
		}
		void Alloc(allocator_t allocator) noexcept(false)
		{
			pair<SystemHeap*, blockid_t> idpair = allocator();

			if (heap != nullptr)
				Drop();

			heap = idpair.former;
			id   = idpair.latter;
		}
		void Alloc(const _T &value, allocator_t allocator) noexcept(false)
		{
			const pair<SystemHeap*, blockid_t> blkid = heap->NewBlock(heap, amount_of_items, flags, destructor);

			if (heap != nullptr)
				Drop();

			heap = idpair.former;
			id   = idpair.latter;

			_T *addr = heap->GetBlockAddress(id);
			*addr = value;
		}
		void Alloc(size_t n, const _T *values, allocator_t allocator) noexcept(false)
		{
			const pair<SystemHeap*, blockid_t> blkid = heap->NewBlock(heap, amount_of_items, flags, destructor);

			if (heap != nullptr)
				Drop();

			heap = idpair.former;
			id   = idpair.latter;

			_T *addr = Address();

			for (size_t i = 0; i < amount_of_items; i++)
				addr[i] = values[i];
		}

		void Realloc(size_t amount_of_items, destructor_t destructor=nullptr) noexcept(false)
		{
			if (amount_of_items < 1) {
				throw logerr(argument_error(
					"Cannot create 0 items inside of a "
					"memory block, argument #2 must be >0."
				));
				return;
			}

			heap->Realloc(id, amount_of_items, destructor);
		}
		void Realloc(SystemHeap::destructor_t destructor) noexcept(false)
		{
			if (heap == nullptr) {
				throw logerr(-1, "Attempted to reallocate a not-yet-allocated memory reference.");
				return;
			} else
				Realloc(heap->GetBlockSize(id), destructor);
		}

		operator size_t() const
		{
			return (heap == nullptr)? 0 : heap->GetBlockSize(id);
		}
		operator const_ptr_t() const
		{
			return Address();
		}
		operator ptr_t()
		{
			return Address();
		}
		
		const_ptr_t Address() const
		{
			return (heap == nullptr)? nullptr : static_cast<const _T*>(heap->GetBlockAddress(id));
		}
		ptr_t       Address()
		{
			if (heap == nullptr)
				throw logerr(nonnull_error("Attempted to dereference a null memref."));
		#if DE_MEMREF_CACHE_ADDR == true
			else if (blkmv_idx != heap->GetBlockMovementIndex())
				addr = static_cast<_T*>(heap->GetBlockAddress(id));

			return *addr;
		#else
			return static_cast<_T*>(heap->GetBlockAddress(id));
		#endif
		}
		const _T* AddrIndex(size_t idx) const
		{
			if (heap == nullptr)
				return nullptr;
		
			const size_t blksz = heap->GetBlockSize(id);
			
			if (idx >= blksz)
				idx = blksz-1;

			const _T* arraddr = static_cast<const _T*>(heap->GetBlockAddress(id));
			return &arraddr[idx];
		}
		_T*       AddrIndex(size_t idx)
		{
			if (heap == nullptr)
				return nullptr;
		
			const size_t blksz = heap->GetBlockSize(id);
			
			if (idx >= blksz)
				idx = blksz-1;

			_T* arraddr = static_cast<_T*>(heap->GetBlockAddress(id));
			return &arraddr[idx];
		}
		const _T& GetIndex(size_t idx) const
		{
			if (heap == nullptr)
				return nullptr;
		
			const size_t blksz = heap->GetBlockSize(id);
			
			if (idx >= blksz)
				idx = blksz-1;

			const _T* arraddr = static_cast<const _T*>(heap->GetBlockAddress(id));
			return arraddr[idx];
		}
		_T&       GetIndex(size_t idx)
		{
			if (heap == nullptr)
				return nullptr;
		
			const size_t blksz = heap->GetBlockSize(id);
			
			if (idx >= blksz)
				idx = blksz-1;

			_T* arraddr = static_cast<_T*>(heap->GetBlockAddress(id));
			return arraddr[idx];
		}

		const _T& operator * (void) const
		{
			const _T *addr = Address();
			if (addr == nullptr)
				throw logerr(nonnull_error("Attempted to deference a nullptr."));
			return *addr;
		}
		_T&       operator * (void)
		{
			_T *addr = Address();
			if (addr == nullptr)
				throw logerr(nonnull_error("Attempted to deference a nullptr."));
			return *addr;
		}
		const _T& operator [](size_t idx) const
		{
			const _T *addr = AddrIndex(idx);
			if (addr == nullptr)
				throw logerr(nonnull_error("Attempted to deference a nullptr."));
			return *addr;
		}
		_T&       operator [](size_t idx)
		{
			_T *addr = AddrIndex(idx);
			if (addr == nullptr)
				throw logerr(nonnull_error("Attempted to deference a nullptr."));
			return *addr;
		}

		virtual int Drop()
		{
			int r = 0;
			if (heap != nullptr)
				r = heap->DropBlock(id);
			heap = nullptr;
			id   = 0;
		#if DE_MEMREF_CACHE_ADDR == true
			addr = nullptr;
		#endif
			return r;
		}

		virtual int Terminate() noexcept override
		{
			try {
				return this->Drop();
			} catch (std::exception e) {
				logerr(e);
				return -1;
			}
		}

		SystemHeap*       GetHeap() noexcept
		{
			return heap;
		}
		const SystemHeap* GetHeap() const noexcept
		{
			return heap;
		}
		blockid_t         GetBlockID()   const noexcept
		{
			return id;
		}
		oid_t             GetReferenceOwner() const noexcept
		{
			return refowner;
		}
		inline oid_t    (&GetRefOwner)() const = GetReferenceOwner;
		oid_t             GetBlockOwner() const noexcept
		{
			if (heap == nullptr)
				return OID_NULL;
			else
				return heap->GetBlockOwner(id);
		}
		int               GetBlockFlags() const noexcept
		{
			if (heap == nullptr)
				return SystemHeap::NULL_FLAGS;
			else
				return heap->GetBlockFlags(id);
		}

		bool IsNull() const noexcept
		{
			return heap == nullptr;
		}
		bool IsActive() const noexcept
		{
			return heap != nullptr;
		}
		bool IsBlockOwner() const noexcept
		{
			return is_block_owner;
		}

		size_t GetArraySize() const noexcept
		{
			return (heap == nullptr)? 0 : (heap->GetBlockSize(id) / sizeof(_T));
		}
		size_t GetArrayBytesLength() const noexcept
		{
			return (heap == nullptr)? 0 : heap->GetBlockSize(id);
		}
		template <typename _U>
		size_t GetArraySizeIfType() const noexcept
		{
			return (heap == nullptr)? 0 : (heap->GetBlockSize(id) / sizeof(_U));
		}
		inline size_t (&GetBlockSize)() const noexcept = GetArrayBytesLength;
		operator size_t() const
		{
			return GetArrayBytesLength();
		}

		int SetBlockFlags(int flags)
		{
			return GetBlockFlags(); // NEEDS IMPLEMENTATION!
		}

		int Borrow(const memref<_T> &borrow)
		{
			if (borrow.IsNull())
				return -1;

			int err = borrow.heap->BorrowBlock(borrow.id);

			if (err == 0) {
				heap           = borrow.heap;
				id             = borrow.id;
				is_block_owner = false;
			} else return -1;
		}
		int SetToCopy(const memref<_T> &copy)
		{
			if (copy.IsNull())
				return -1;

			blockid_t   block;
			SystemHeap *heap_ref = (heap == nullptr)? &SystemHeap::GetCurrent() : heap;

			const size_t blklen = copy.GetArrayBytesLength();

			try {
				block = heap_ref->NewBlock(blklen);
				if (heap != nullptr)
					Drop();
				id   = block;
				heap = heap_ref;

				const size_t arrlen  = blklen / sizeof(_T);
				_T*          arrdest = *this;
				const _T*    arrsrc  = copy;
				for (size_t i = 0; i < arrlen; i++)
					arrdest[i] = arrsrc[i];
				
				return 0;
			} catch (std::exception e) {
				logerr(e);
				return -1;
			}
		}

		memref<_T>& operator =(memref<_T> &rhv)
		{
			Borrow(rhv);
		}
		memref<_T>& operator =(const memref<_T> &rhv)
		{
			Borrow(rhv);
		}

		bool TransferOwnership(memref<_T> *transfer_to)
		{
			if (transfer_to == nullptr || !is_block_owner)
				return false;

			else if (transfer_to->Drop() != 0) {
				logerr(-1, "Failed to drop the target memref.");
				return false;
			}

			int err = heap->TransferBlockOwnership(id, transfer_to->refowner);
			if (err != 0)
				goto early_return;

			transfer_to->heap           = heap;
			transfer_to->id             = id;
			transfer_to->is_block_owner = true;
			is_block_owner              = false;

		early_return:
			return err;
		}

		template <typename _ReTyp>
		typename std::enable_if_t<
				is_reinterpretable_v<_T, _ReTyp> && is_reinterpretable_v<_ReTyp, _T>
			int>
		ReinterpretOwnership(memref<_ReTyp> *transfer_to)
		{
			if (transfer_to == nullptr || !is_block_owner)
				return false;

			else if (transfer_to->Drop() != 0) {
				logerr(-1, "Failed to drop the target memref.");
				return false;
			}

			int err = heap->TransferBlockOwnership(id, transfer_to->refowner);
			if (err != 0)
				goto early_return;

			transfer_to->heap           = heap;
			transfer_to->id             = id;
			transfer_to->is_block_owner = true;
			is_block_owner              = false;

		early_return:
			return err;
		}

		template <typename _ReTyp>
		typename std::enable_if_t<is_reinterpretable_v<_T, _ReTyp>, int>
		ReinterpretType(memref<_ReTyp> *output)
		{
			if (output == nullptr) {
				return -1;
			} else if (heap == nullptr) {
				output->Drop();
				output->id   = 0;
				output->heap = nullptr;
			#if DE_MEMREF_CACHE_ADDR == true
				output->addr = nullptr;
			#endif
				output->is_block_owner = false;
				return 0;
			} else {
				output->Drop();
				*output = memref<_NewTyp>();
				output->heap = heap;
				output->id   = id;
				output->is_block_owner = false;
			#if DE_MEMREF_CACHE_ADDR == true
				output->addr = nullptr;
				addr = nullptr;
			#endif
				return 0;
			}
		}
	};

	template <typename _T>
	class memview: public iIterable<_T>
	{
		memref<_T>       *ref;
		const memref<_T> *const_ref;
		size_t            viewidx;
		oid_t             viewowner;

	public:
		typedef _ReTyp       *ptr_t;
		typedef const _ReTyp *const_ptr_t;

		memview():
			ref(nullptr), const_ref(nullptr)
		{
			viewidx   = 0;
			viewowner = GetCurrentOID();
		}
		memview(const memview<_T> &rhv):
			ref(nullptr), const_ref(rhv.const_ref)
		{
			viewidx   = rhv.viewidx;
			viewowner = GetCurrentOID();
		}
		memview(memview<_T> &rhv):
			ref(rhv.ref), const_ref(rhv.const_ref)
		{
			viewidx   = rhv.viewidx;
			viewowner = GetCurrentOID();
		}
		memview(const memref<_T> &ref, size_t view_index=0):
			ref(nullptr), const_ref(&ref)
		{
			const size_t arrsz = ref.GetArraySize();

			viewidx   = (view_index >= arrsz)? arrsz - 1 : view_index;
			viewowner = GetCurrentOID();
		}
		memview(memref<_T> &ref, size_t view_index=0):
			ref(&ref), const_ref(&ref)
		{
			const size_t arrsz = ref.GetArraySize();

			viewidx   = (view_index >= arrsz)? arrsz - 1 : view_index;
			viewowner = GetCurrentOID();
		}

		static memview<_T> Copy(memview<_T> &rhv)
		{
			memview<_T> r(rhv);

			r.ref = rhv.ref;

			return r;
		}
		static memview<_T> View(memref<_T> &ref, size_t view_index=0)
		{
			memview<_T> r(ref, view_index);

			r.ref = rhv.ref;

			return r;
		}

		~memview()
		{}

		virtual int Drop()
		{
			if (ref == nullptr) {
				if (const_ref == nullptr)
					return 0;
				const SystemHeap *heap = const_ref->GetHeap();

				return heap->DropBlock(const_ref->GetBlockID());
			} else {
				SystemHeap *heap = ref->GetHeap();

				return heap->DropBlock(ref->GetBlockID());
			}
		}

		virtual int Terminate() noexcept override
		{
			try {
				return this->Drop();
			} catch (std::exception e) {
				logerr(e);
				return -1;
			}
		}

		memview<_T> Copy()
		{
			return memview<_T>::View(*ref, viewidx);
		}
		memview<_T> Copy() const
		{
			return memview<_T>::View(*const_ref, viewidx);
		}

		memview<_T>& operator =(memview<_T> &rhv)
		{
			ref     = rhv.ref;
			viewidx = rhv.viewidx;
		}

		bool operator ==(const memview<_T> &rhv) const
		{
			return ref == rhv.ref && viewidx == rhv.viewidx;
		}
		bool operator !=(const memview<_T> &rhv) const
		{
			return ref != rhv.ref || viewidx != rhv.viewidx;
		}
		bool operator <=(const memview<_T> &rhv) const
		{
			return viewidx <= rhv.viewidx;
		}
		bool operator >=(const memview<_T> &rhv) const
		{
			return viewidx >= rhv.viewidx;
		}
		bool operator  <(const memview<_T> &rhv) const
		{
			return viewidx < rhv.viewidx;
		}
		bool operator  >(const memview<_T> &rhv) const
		{
			return viewidx > rhv.viewidx;
		}
		bool operator ==(const memref<_Original> &rhv) const
		{
			return ref == rhv;
		}
		bool operator !=(const memref<_Original> &rhv) const
		{
			return ref != rhv;
		}

		bool operator  !(const memview<_T> &rhv) const
		{
			return ref.IsNull();
		}
		operator bool() const
		{
			return !ref.IsNull();
		}

		operator size_t()  const
		{
			return viewidx;
		}
		operator ptr_t() const
		{
			return &(ref.GetIndex(viewidx));
		}
		operator const_ptr_t() const
		{
			return &(ref.GetIndex(viewidx));
		}

		const_ptr_t Address() const
		{
			return &(ref.GetIndex(viewidx));
		}
		ptr_t Address()
		{
			return &(ref.GetIndex(viewidx));
		}
		const_ptr_t AddrIndex(size_t idx) const
		{
			return ref.AddrIndex(viewidx+idx);
		}
		ptr_t AddrIndex(size_t idx)
		{
			return ref.AddrIndex(viewidx+idx);
		}
		const _T& GetIndex(size_t idx) const
		{
			return ref.GetIndex(viewidx+idx);
		}
		_T& GetIndex(size_t idx)
		{
			return ref.GetIndex(viewidx+idx);
		}

		const _T& operator *(void) const
		{
			return *(ref.GetConstIndex(viewidx));
		}
		_T& operator *(void)
		{
			return *(ref.GetIndex(viewidx));
		}
		const _T& operator [](size_t idx) const
		{
			return *(ref.GetConstIndex(viewidx+idx));
		}
		_T& operator [](size_t idx)
		{
			return *(ref.GetIndex(viewidx+idx));
		}

		memview<_T> operator +(size_t idx) const
		{
			return memview<_T>(ref, viewidx+idx);
		}
		memview<_T> operator -(size_t idx) const
		{
			return memview<_T>(ref, (idx >= viewidx)? 0 : viewidx-idx);
		}
		memview<_T>& operator +=(size_t idx)
		{
			const size_t idxf = idx + viewidx, arrsz = ref.GetArraySize();

			if (idxf >= arrsz)
				idxf = arrsz - 1;

			viewidx = idxf;
		}
		memview<_T>& operator -=(size_t idx)
		{
			if (idx >= viewidx)
				viewidx = 0;
			else
				viewidx += idx;
		}

		memref<_T>&       Ref()
		{
			return ref;
		}
		const memref<_T>& Ref() const
		{
			return ref;
		}
		size_t            GetViewingIndex()  const
		{
			return viewidx;
		}
		oid_t             GetViewOwner() const
		{
			return viewowner;
		}
		oid_t             GetRefOwner() const
		{
			return ref.GetReferenceOwner();
		}

		bool IsNull() const
		{
			return ref.IsNull();
		}
		bool IsActive() const
		{
			return ref.IsActive();
		}

		int Adopt(memref<_T> &rhv, size_t viewing_index=0)
		{
			ref       = &rhv;
			const_ref = &rhv;
			viewidx   = viewing_index;
		}
		int Adopt(const memref<_T> &rhv, size_t viewing_index=0)
		{
			ref       = nullptr;
			const_ref = &rhv;
			viewidx   = viewing_index;
		}
	};
	template <typename _Original, typename _ReType>
	class reinterpreted_memview: public iIterable<_ReTyp>
	{
		static_assert(
			is_reinterpretable_v<_Original, _ReTyp>,
			"The original type of this reference is not reinterpretable as the given type."
		);

		memref<_Original>       *ref;
		const memref<_Original> *const_ref;
		size_t                   viewidx;
		oid_t                    viewowner;

	public:

		typedef _ReTyp       *ptr_t;
		typedef const _ReTyp *const_ptr_t;
		typedef _ReTyp       *uncasted_ptr_t;
		typedef const _ReTyp *const_uncasted_ptr_t;

		reinterpreted_memview():
			ref(nullptr), const_ref(nullptr)
		{
			viewidx   = 0;
			viewowner = GetCurrentOID();
		}
		reinterpreted_memview(const reinterpreted_memview<_Original,_ReTyp>&):
			ref(nullptr), const_ref(rhv.const_ref)
		{
			viewidx   = rhv.viewidx;
			viewowner = GetCurrentOID();
		}
		reinterpreted_memview(reinterpreted_memview<_Original,_ReTyp> &rhv):
			ref(rhv.ref), const_ref(rhv.const_ref)
		{
			viewidx   = rhv.viewidx;
			viewowner = GetCurrentOID();
		}

		reinterpreted_memview(memref<_Original> &ref, size_t viewing_index=0):
			ref(&ref), const_ref(&ref)
		{
			viewidx   = viewing_index;
			viewowner = GetCurrentOID();
		}
		reinterpreted_memview(const memref<_Original> &ref, size_t viewing_index=0):
			ref(nullptr), const_ref(&ref)
		{
			viewidx   = viewing_index;
			viewowner = GetCurrentOID();
		}

		~reinterpreted_memview()
		{}

		virtual int Drop()
		{
			if (ref == nullptr) {
				if (const_ref == nullptr)
					return 0;
				const SystemHeap *heap = const_ref->GetHeap();

				return heap->DropBlock(const_ref->GetBlockID());
			} else {
				SystemHeap *heap = ref->GetHeap();

				return heap->DropBlock(ref->GetBlockID());
			}
		}

		virtual int Terminate() noexcept override
		{
			try {
				return this->Drop();
			} catch (std::exception e) {
				logerr(e);
				return -1;
			}
		}

		memview<_T>& operator =(const reinterpreted_memview<_Original,_ReTyp> &rhv)
		{
			ref       = nullptr;
			const_ref = rhv.const_ref;
			viewidx   = rhv.viewidx;
		}
		memview<_T>& operator =(reinterpreted_memview<_Original,_ReTyp> &rhv)
		{
			ref       = rhv.ref;
			const_ref = rhv.const_ref;
			viewidx   = rhv.viewidx;
		}

		bool operator ==(const reinterpreted_memview<_Original,_ReTyp> &rhv) const
		{
			return ref == rhv.ref && viewidx == rhv.viewidx;
		}
		bool operator !=(const reinterpreted_memview<_Original,_ReTyp> &rhv) const
		{
			return ref != rhv.ref || viewidx != rhv.viewidx;
		}
		bool operator <=(const reinterpreted_memview<_Original,_ReTyp> &rhv) const
		{
			return viewidx <= rhv.viewidx;
		}
		bool operator >=(const reinterpreted_memview<_Original,_ReTyp> &rhv) const
		{
			return viewidx >= rhv.viewidx;
		}
		bool operator  <(const reinterpreted_memview<_Original,_ReTyp> &rhv) const
		{
			return viewidx < rhv.viewidx;
		}
		bool operator  >(const reinterpreted_memview<_Original,_ReTyp> &rhv) const
		{
			return viewidx > rhv.viewidx;
		}

		bool operator ==(const memview<_Original> &rhv) const
		{
			return ref == rhv.Ref() && viewidx == rhv.GetViewingIndex();
		}
		bool operator !=(const memview<_Original> &rhv) const
		{
			return ref != rhv.Ref() || viewidx != rhv.GetViewingIndex();
		}
		bool operator <=(const memview<_Original> &rhv) const
		{
			return viewidx <= rhv.GetViewingIndex();
		}
		bool operator >=(const memview<_Original> &rhv) const
		{
			return viewidx >= rhv.GetViewingIndex();
		}
		bool operator  <(const memview<_Original> &rhv) const
		{
			return viewidx < rhv.GetViewingIndex();
		}
		bool operator  >(const memview<_Original> &rhv) const
		{
			return viewidx > rhv.GetViewingIndex();
		}

		bool operator ==(const memref<_Original> &rhv) const
		{
			return ref == rhv;
		}
		bool operator !=(const memref<_Original> &rhv) const
		{
			return ref != rhv;
		}

		bool operator  !(void) const
		{
			return const_ref->IsNull();
		}
		operator bool() const
		{
			return !const_ref->IsNull();
		}

		bool IsNull() const
		{
			return const_ref != nullptr && const_ref->IsNull();
		}
		bool IsActive() const
		{
			return const_ref != nullptr && const_ref->IsActive();
		}
		bool IsConst() const
		{
			return ref == nullptr || ref->IsNull();
		}
		bool IsWritable() const
		{
			return ref != nullptr && !ref->IsNull();
		}

		constexpr operator size_t()  const
		{
			return viewidx;
		}
		constexpr operator ptr_t()
		{
			if (ref == nullptr)
				return nullptr;
			return reinterpret_cast<ReTyp*>(Ref().AddrIndex(viewidx));
		}
		constexpr operator const_ptr_t() const
		{
			return reinterpret_cast<const _ReTyp*>(Ref().AddrIndex(viewidx));
		}
		constexpr operator uncasted_ptr_t()
		{
			if (ref == nullptr)
				return nullptr;
			return Ref().Address();
		}
		constexpr operator const_uncasted_ptr_t() const
		{
			return Ref().Address();
		}

		const_ptr_t Address() const
		{
			return reinterpret_cast<const ReTyp*>(Ref().AddrIndex(viewidx));
		}
		ptr_t Address()
		{
			if (ref == nullptr)
				return nullptr;
			return reinterpret_cast<_ReTyp*>(Ref().AddrIndex(viewidx));
		}
		const_ptr_t AddrIndex(size_t idx) const
		{
			return Ref().AddrIndex(viewidx+idx);
		}
		ptr_t AddrIndex(size_t idx)
		{
			if (ref == nullptr)
				return nullptr;
			return Ref().AddrIndex(viewidx+idx);
		}
		const _T& GetIndex(size_t idx) const
		{
			return Ref().GetIndex(viewidx+idx);
		}
		_T& GetIndex(size_t idx)
		{
			if (ref == nullptr)
				return nullptr;
			return Ref().GetIndex(viewidx+idx);
		}

		const _ReTyp& operator *(void) const
		{
			return *(Ref().GetIndex(viewidx));
		}
		_ReTyp& operator *(void)
		{
			if (ref == nullptr)
				return nullptr;
			return *(Ref().GetIndex(viewidx));
		}
		const _ReTyp& operator [](size_t idx) const
		{
			return *(Ref().GetIndex(viewidx+idx));
		}
		_ReTyp& operator [](size_t idx)
		{
			if (ref == nullptr)
				return nullptr;
			return *(Ref().GetIndex(viewidx+idx));
		}

		rememview<_Original, _ReTyp> operator +(size_t idx) const
		{
			return rememview<_Original, _ReTyp>(ref, viewidx+idx);
		}
		rememview<_Original, _ReTyp> operator -(size_t idx) const
		{
			return rememview<_Original, _ReTyp>(ref, (idx >= viewidx)? 0 : viewidx-idx);
		}
		rememview<_Original, _ReTyp>& operator +=(size_t idx)
		{
			const size_t idxf = idx + viewidx, arrsz = ref.GetArraySizeIfType<_ReTyp>();

			if (idxf >= arrsz)
				idxf = arrsz - 1;

			viewidx = idxf;
		}
		rememview<_Original, _ReTyp>& operator -=(size_t idx)
		{
			if (idx >= viewidx)
				viewidx = 0;
			else
				viewidx += idx;
		}

		memref<_Original>&       Ref()
		{
			return ref;
		}
		const memref<_Original>& Ref() const
		{
			return ref;
		}
		size_t GetViewingIndex()  const
		{
			return viewidx;
		}
		oid_t  GetViewOwner() const
		{
			return viewowner;
		}
		oid_t  GetRefOwner() const
		{
			return ref.GetReferenceOwner();
		}

		bool IsNull() const
		{
			return ref.IsNull();
		}
		bool IsActive() const
		{
			return ref.IsActive();
		}

		int Adopt(memref<_Original> &rhv, size_t viewing_index=0)
		{
			ref       = &rhv;
			const_ref = &rhv;
			viewidx   = viewing_index;
		}
		int Adopt(const memref<_Original> &rhv, size_t viewing_index=0)
		{
			ref       = nullptr;
			const_ref = &rhv;
			viewidx   = viewing_index;
		}

		memview<_Original> Deinterpret() const
		{
			return memview<_Original>(ref, viewidx);
		}
	};
	template <typename _Original, typename _ReTyp>
	using rememview = reinterpreted_memview<_Original, _ReTyp>;

	template <typename _Original, typename _ReTyp>
	typename std::enable_if_t<is_reinterpretable_v<_Original, _ReTyp>, bool>
	operator ==(const memview<_Original> &lhv, const rememview<_Original, _ReTyp> &rhv)
	{
		return lhv.Ref() == rhv.Ref() && lhv.GetViewingIndex() == rhv.GetViewingIndex();
	}
	template <typename _Original, typename _ReTyp>
	typename std::enable_if_t<is_reinterpretable_v<_Original, _ReTyp>, bool>
	operator !=(const memview<_Original> &lhv, const rememview<_Original, _ReTyp> &rhv)
	{
		return lhv.Ref() != rhv.Ref() || lhv.GetViewingIndex() != rhv.GetViewingIndex();
	}
	template <typename _Original, typename _ReTyp>
	typename std::enable_if_t<is_reinterpretable_v<_Original, _ReTyp>, bool>
	operator <=(const memview<_Original> &lhv, const rememview<_Original, _ReTyp> &rhv)
	{
		return viewidx <= rhv.GetViewingIndex();
	}
	template <typename _Original, typename _ReTyp>
	typename std::enable_if_t<is_reinterpretable_v<_Original, _ReTyp>, bool>
	operator >=(const memview<_Original> &lhv, const rememview<_Original, _ReTyp> &rhv)
	{
		return viewidx >= rhv.GetViewingIndex();
	}
	template <typename _Original, typename _ReTyp>
	typename std::enable_if_t<is_reinterpretable_v<_Original, _ReTyp>, bool>
	operator  <(const memview<_Original> &lhv, const rememview<_Original, _ReTyp> &rhv)
	{
		return viewidx < rhv.GetViewingIndex();
	}
	template <typename _Original, typename _ReTyp>
	typename std::enable_if_t<is_reinterpretable_v<_Original, _ReTyp>, bool>
	operator  >(const memview<_Original> &lhv, const rememview<_Original, _ReTyp> &rhv)
	{
		return viewidx > rhv.GetViewingIndex();
	}
}



#endif // __DEMONEYE__setup_MEMORY_H
