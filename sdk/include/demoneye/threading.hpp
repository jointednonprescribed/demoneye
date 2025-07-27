
#ifndef __DEMONEYE__THREADING_H
#define __DEMONEYE__THREADING_H 1

#include "memory.hpp"



namespace demoneye
{
	/* System Thread Handle type */
#if DE_PLATFORM == DE_WINDOWS
	typedef HANDLE threadhandle_t;
#	if DE_PLATFORM32 == true
	constexpr threadhandle_t NULL_THREADHANDLE = 0xffffffff; //         -1
#	else // PLATFORM64
	constexpr threadhandle_t NULL_THREADHANDLE = 0xffffffffffffffff; // -1
#	endif
#elif DE_PLATFORM_UNIXLIKE == true
	typedef pid_t  threadhandle_t;
	constexpr threadhandle_t NULL_THREADHANDLE = NULL_PID;
#endif

	class thread: public iTerminatable
	{
	public:
		enum exit_protocol
		{
			/* Add the thread to the global thread terminator list and ignore
			.  if its still running, if the thread terminator list is full,
			.  the thread will instead be terminated directly, if it's still
			.  running, upon destruction of the thread object. */
			ADD_TO_TERM_LIST,

			/* Terminate upon thread object destruction. */
			TERMINATE,

			/* Wait for the thread to finish before moving on. */
			WAIT,

			/* Leave the process detached from the parent and let it run until
			.  it dies (the code will kill it when it returns from it's function). */
			LEAVE_DETACHED,

			DEFAULT_EXIT_PROC = ADD_TO_TERM_LIST,
		};

		enum status
		{
			UNINITIALIZED,
			INITIALIZED,
			STOPPED,
			SUSPENDED,
			RUNNING,
		};

		typedef lambda start_routine;

	private:
		start_routine  entrypoint;
		exit_protocol  exitproc;
		status         stat;
		pid_t          procid;
	#if DE_PLATFORM == DE_WINDOWS
		HANDLE         handle;
	#endif

	public:

		thread() noexcept;
		explicit thread(start_routine entry, exit_protocol exit=DEFAULT_EXIT_PROC, bool start=true) noexcept(false);
		~thread() noexcept;

		const start_routine& EntryPoint()      const noexcept;
		exit_protocol        ExitProtocol()    const noexcept;
		pid_t                ProcessID()       const noexcept;
		status               Status()          const noexcept;
		threadhandle_t       Handle()          const noexcept;

		virtual int   Terminate() override;

		status        Stop()                   noexcept(false);
		status        Suspend()                noexcept(false);
		status        Resume()                 noexcept(false);
		pid_t         Start()                  noexcept(false);
		pid_t         Start(start_routine entrypoint) noexcept(false);

		void          Wait()             noexcept;
		void          Wait(long long ms) noexcept;

		bool          SetEntryPoint(start_routine set_to) noexcept;
		bool          SetExitProtocol(exit_protocol set_to) noexcept;
		status        SetStatus(status status);

		bool          IsInitialized() const noexcept;
		bool          IsStopped()     const noexcept;
		bool          IsSuspended()   const noexcept;
		bool          IsSuspendedOrStopped() const noexcept;
		bool          IsRunning()     const noexcept;
	};

	class subprocess: public thread
	{
	public:
		
		subprocess();
		~subprocess();

		virtual int Terminate() override;
	};

	class coroutine: public thread
	{
	public:

		coroutine();
		~coroutine();

		virtual int Terminate() override;
	};

	class event
	{
	public:

		event();
		~event();
	};
	class event_loop: public thread
	{
	public:
		
		event_loop();
		~event_loop();

		virtual int Terminate() override;
	};
}



#endif // __DEMONEYE__THREADING_H
