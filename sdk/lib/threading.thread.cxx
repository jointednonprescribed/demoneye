


/* Implementing: demoneye/threading.hpp: class thread */
#include <demoneye/threading.hpp>

/* Source Requires: demoneye/logger.hpp */
#include <demoneye/logger.hpp>



namespace demoneye
{
	thread::thread()
	{
		entrypoint = NULL;
		exitproc   = DEFAULT_EXIT_PROC;
		stat       = UNINITIALIZED;
		procid     = NULL_PID;
	}
	thread::thread(start_routine entry, exit_protocol exit, bool start)
	{
		entrypoint = entry;
		exitproc   = exit;
		procid     = start? Start() : 0;
		stat       = (procid == 0)? INITIALIZED : RUNNING;
	}

	thread::~thread()
	{
		if (IsRunning()) {
		reswitch:
			switch(exitproc) {
				case ADD_TO_TERM_LIST:
					// IMPLEMENTING ENGINE CONTEXT AND THREAD TERMINATION LIST,
					// use iTermatable interface for a list of terminatable
					// resources like memory, threads, and filesystem resources.
					break;

				case TERMINATE:
					if (Stop() != STOPPED) {
						logwarnf("Failed to terminate thread subprocess %lu, trying again...",
							static_cast<unsigned long>(procid));
						if (Stop() != STOPPED) {
							logwarnf(
								"Termination of thread subprocess %lu could not be performed, "
								"it may still be running...",
								static_cast<unsigned long> (procid));
						} else
							logf("Successfully terminated subprocess %lu!", static_cast<unsigned long>(procid));
					}
					break;

				case LEAVE_DETACHED:
					logwarnf("Leaving thread subprocess %lu detached as per exit protocol.",
						static_cast<unsigned long> (procid));
					break;

				case WAIT:
					logf("Waiting for thread subprocess %lu to finish...", static_cast<unsigned long>(procid));
					Wait();
					break;

				default:
					logwarnf("Unexpected thread exit protocol value %d, correcting to default...",
						static_cast<int>(exitproc));
					exitproc = DEFAULT_EXIT_PROC;
					goto reswitch;
			}
		} else if (stat == SUSPENDED) {
			try {
				SetStatus(STOPPED);
			} catch (std::exception e) {
				logwarnf("Failed to stop thread subprocess %lu, %r",
					static_cast<unsigned long> (procid),
					static_cast<const std::exception&> (e)
				);
			}
		}
		entrypoint = nullptr;
		exitproc   = DEFAULT_EXIT_PROC;
		stat       = UNINITIALIZED;
		procid     = NULL_PID;
	}

	const thread::start_routine& thread::EntryPoint() const
	{
		return entrypoint;
	}
	thread::exit_protocol thread::ExitProtocol() const
	{
		return exitproc;
	}
	pid_t thread::ProcessID() const
	{
		return procid;
	}
	thread::status thread::Status() const
	{
		return stat;
	}

	void thread::Wait()
	{
		while (IsRunning());
	}
	void thread::Wait(long long ms)
	{
		if (!IsRunning())
			return;

		const pid_t pid = procid;

		int64_t now = static_cast<decltype(now)>(std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count()),
		then = now + ms;

		while (true) {
			if (IsStopped()) {
				logf("Thread subprocess %lu finished.", static_cast<unsigned long>(pid));
				break;
			}

			now = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			).count();
			if (now >= then) {
				logwarnf("Timed out waiting for thread subprocess %lu to finish...",
					static_cast<unsigned long>(pid));
				break;
			}
		}
	}

	thread::status thread::Stop()
	{

	}
	thread::status thread::Suspend()
	{

	}
	thread::status thread::Resume()
	{

	}
	pid_t thread::Start()
	{
		pid_t pid = 0;

		if (stat == RUNNING) {
			logwarn("Cannot start a thread that is already running, ignoring...");
			return procid;
		} else if (stat == SUSPENDED) {
			logwarnf("Thread subprocess %lu is already active, but is suspended, attempting to resume...");
			Resume();
			return procid;
		}
	#if DE_PLATFORM == DEMONEYE_WINDOWS
	/* ---  WINDOWS IMPLEMENTATION --- */
		HANDLE hThread = CreateThread(NULL, 0, entrypoint.Address(), NULL, 0);
		handle = hThread;

		if (hThread != NULL_THREADHANDLE) {
			stat   = RUNNING;
			pid    = GetProcessIdOfThread(hThread);
			procid = pid;
		} else {
			throw logerr(runtime_error("Unexpected error -- failed to start thread subprocess..."));
		}

	#elif DE_PLATFORM_UNIXLIKE == true
	/* ---  UNIX IMPLEMENTATION --- */
		pid  = fork();
		stat = RUNNING;
		if (pid == 0) { // child process:
			pid    = getpid();
			procid = pid;
			entrypoint();
			kill(pid, SIGTERM); // call for self-termination
			kill(pid, SIGKILL); // if self-termination does not exit, kill signal
			//                     will force it to.
			pid = 0; // just in case
		} else { //        parent process:
			procid = pid;
		}
	#endif
		return pid;
	}
	pid_t thread::Start(start_routine entry)
	{
		if (stat == RUNNING) {
			logwarn("Cannot start a thread that is already running, ignoring...");
			return procid;
		} else if (stat == SUSPENDED) {
			logwarnf("Thread subprocess %lu is already active, but is suspended, attempting to terminate...");
			Stop();
		}
		if (!SetEntryPoint(entry)) {
			throw logerr(runtime_error("Failed to set the entry point of the thread to the supplied address."));
			return procid;
		}

		Start();

		return procid;
	}

	bool thread::SetEntryPoint(start_routine entry)
	{
		if (entry != nullptr) {
			entrypoint = entry;
		}
	}
	bool thread::SetExitProtocol(exit_protocol exit)
	{
		exitproc = exit;
	}
	thread::status thread::SetStatus(status setstat)
	{
		switch (setstat) {

			case UNINITIALIZED:
				if (stat == RUNNING || stat == SUSPENDED) {
					Stop();
				}
				entrypoint = nullptr;
				stat       = UNINITIALIZED;
				procid     = NULL_PID;
				break;

			case INITIALIZED:
				if (stat == UNINITIALIZED) {
					throw logerr(runtime_error(
						"Cannot set a thread as initialized without an entrypoint being initialized first."));
				}
				break;

			case STOPPED:
				if (stat == RUNNING || stat == SUSPENDED) {
					Stop();
					break;
				} else if (stat == INITIALIZED) {
					stat = STOPPED;
				} else if (stat == UNINITIALIZED)
					throw logerr(runtime_error("Cannot stop an uninitialized thread."));
				break;

			case SUSPENDED:
				if (stat == RUNNING) {
					Suspend();
					break;
				} else if (stat != SUSPENDED)
					throw logerr(runtime_error(
						"Cannot set a thread to suspended status unless it's already running."));
				break;

			case RUNNING:
				if (stat != RUNNING) {
					if (stat == UNINITIALIZED) {
						throw logerr(runtime_error("Cannot start an uninitialized thread!"));
						break;
					} else if (stat == SUSPENDED) {
						Resume();
						break;
					} else {
						Start();
						break;
					}
				}
				else break;
		}
		return stat;
	}

	bool thread::IsInitialized() const
	{
		return entrypoint != nullptr && stat == INITIALIZED;
	}
	bool thread::IsStopped() const
	{
		return stat == STOPPED;
	}
	bool thread::IsSuspended() const
	{
		return stat == SUSPENDED;
	}
	bool thread::IsSuspendedOrStopped() const
	{
		return stat == SUSPENDED || stat == STOPPED;
	}
	bool thread::IsRunning() const
	{
		return stat == RUNNING;
	}
}


