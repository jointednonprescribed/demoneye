


/* Implementing: demoneye/env.hpp: class EngineContext */
#include <demoneye/env.hpp>



namespace demoneye
{
	EngineContext::EngineContext():
		glfw(), logger(), restable(), termlist(DEFAULT_TERMLIST_SIZE)
	{}
	EngineContext::EngineContext(int subsys):
		glfw(), logger(), restable(), termlist(DEFAULT_TERMLIST_SIZE)
	{
		InitSubsystems(subsys);
	}

	EngineContext::~EngineContext()
	{
		bool failed = false;
		int err = 0;

		try {
			err = Terminate();
			if (err != 0) {
				failed = true;
				logwarnf(
					"Termination subroutine for Engine Context at <%p> returned non-zero "
					"error code (%d),\nsome subsystems may have failed to terminate, retrying...",
					static_cast<void*>(this), err);
			} else {
				logf("Successfully terminated EngineContext at <%p>!", static_cast<void*>(this));
				return;
			}
		} catch (exception e) {
			failed = true;
		} catch (std::exception e) {
			failed = true;
			logerr(e);
		}

		if (failed) {
			logwarn("Attempting to retry EngineContext termination...");
			try {
				err = Terminate();
				if (err != 0) {
					logerrf(
						err,
						"Termination subroutine for Engine Context at <%p> returned non-zero "
						"error code (%d),\nsome subsystems may have failed to terminate.",
						static_cast<void*>(this), err);
				} else
					logf("Successfully terminated EngineContext at <%p> on the second attempt!",
						static_cast<void*>(this));
				return;
			} catch (exception e) {
				goto fail_resolution;
			} catch (std::exception e) {
				logerr(e);
				goto fail_resolution;
			}
		fail_resolution:
			logwarnf("Failed to terminate EngineContext at <%p>, unexpected error occurred!",
				static_cast<void*>(this));
			return;
		}
	}


	int EngineContext::InitMemorySubsystem()
	{
		return 1;
	}
	int EngineContext::InitThreadSubsystem()
	{
		return 1;
	}
	int EngineContext::InitIOSubsystem()
	{
		logger.SetOutputPipe(logger.GetStdout());
		logger.SetErrorPipe(logger.GetStderr());
	}
	int EngineContext::InitPeripheralSubsystem()
	{
		return 1;
	}
	int EngineContext::InitVideoSubsystem()
	{
		return 1;
	}
	int EngineContext::InitAudioSubsystem()
	{
		return 1;
	}
	int EngineContext::InitAllSubsystems()
	{
		return
			InitMemorySubsystem()     ||
			InitThreadSubsystem()     ||
			InitIOSubsystem()         ||
			InitPeripheralSubsystem() ||
			InitVideoSubsystem()      ||
			InitAudioSubsystem();
	}
	int EngineContext::InitSubsystems(int subsys)
	{
		bool full_success = true;

		int
			r = 0,
			m = subsys & _SUBSYS_BIT_MEMORY,
			t = subsys & _SUBSYS_BIT_THREAD,
			i = subsys & _SUBSYS_BIT_IO,
			p = subsys & _SUBSYS_BIT_PERIPH,
			v = subsys & _SUBSYS_BIT_VIDEO,
			a = subsys & _SUBSYS_BIT_AUDIO;

		if (m == 0)
			goto threading;
		try {
			r = InitMemorySubsystem();
			if (r == 0)
				goto threading;
		} catch (std::exception e) {
			logerr(e);
		} catch (exception e) {}
		logwarnf("Failed to initialize memory subsystem (error code %d).", r);
		full_success = false;
		r = 0;
	threading:
		if (t == 0)
			goto io;
		try {
			r = InitThreadSubsystem();
			if (r == 0)
				goto io;
		} catch (std::exception e) {
			logerr(e);
		} catch (exception e) {}
		logwarnf("Failed to initialize thread subsystem (error code %d).", r);
		full_success = false;
		r = 0;
	io:
		if (i == 0)
			goto periph;
		try {
			r = InitIOSubsystem();
			if (r == 0)
				goto periph;
		} catch (std::exception e) {
			logerr(e);
		} catch (exception e) {}
		logwarnf("Failed to initialize IO subsystem (error code %d).", r);
		full_success = false;
		r = 0;
	periph:
		if (p == 0)
			goto video;
		try {
			r = InitPeripheralSubsystem();
			if (r == 0)
				goto video;
		} catch (std::exception e) {
			logerr(e);
		} catch (exception e) {}
		logwarnf("Failed to initialize peripheral subsystem (error code %d).", r);
		full_success = false;
		r = 0;
	video:
		if (v == 0)
			goto audio;
		try {
			r = InitVideoSubsystem();
			if (r == 0)
				goto audio;
		} catch (std::exception e) {
			logerr(e);
		} catch (exception e) {}
		logwarnf("Failed to initialize video subsystem (error code %d).", r);
		full_success = false;
		r = 0;
	audio:
		if (a == 0)
			goto returnblk;
		try {
			r = InitAudioSubsystem();
			if (r == 0)
				goto returnblk;
		} catch (std::exception e) {
			logerr(e);
		} catch (exception e) {}
		logwarnf("Failed to initialize audio subsystem (error code %d).", r);
		full_success = false;
		r = 0;
	returnblk:
		return r;
	}

	int EngineContext::TerminateMemorySubsystem()
	{}
	int EngineContext::TerminateThreadSubsystem()
	{}
	int EngineContext::TerminateIOSubsystem()
	{}
	int EngineContext::TerminatePeripheralSubsystem()
	{}
	int EngineContext::TerminateVideoSubsystem()
	{}
	int EngineContext::TerminateAudioSubsystem()
	{}
	int EngineContext::TerminateAllSubsystems()
	{
		return
			TerminateMemorySubsystem()     ||
			TerminateThreadSubsystem()     ||
			TerminateIOSubsystem()         ||
			TerminatePeripheralSubsystem() ||
			TerminateVideoSubsystem()      ||
			TerminateAudioSubsystem();
	}
	int EngineContext::TerminateSubsystems(int subsys)
	{
		int
			r = 0,
			m = subsys & _SUBSYS_BIT_MEMORY,
			t = subsys & _SUBSYS_BIT_THREAD,
			i = subsys & _SUBSYS_BIT_IO,
			p = subsys & _SUBSYS_BIT_PERIPH,
			v = subsys & _SUBSYS_BIT_VIDEO,
			a = subsys & _SUBSYS_BIT_AUDIO;

		if (m == 0)
			goto threading;
		r = TerminateMemorySubsystem() != 0;
	threading:
		if (t == 0)
			goto io;
		r = TerminateThreadSubsystem() != 0;
	io:
		if (i == 0)
			goto periph;
		r = TerminateIOSubsystem() != 0;
	periph:
		if (p == 0)
			goto video;
		r = TerminatePeripheralSubsystem() != 0;
	video:
		if (v == 0)
			goto audio;
		r = TerminateAudioSubsystem() != 0;
	audio:
		if (a == 0)
			goto returnblk;
		r = TerminateAudioSubsystem() != 0;
	returnblk:
		return r;
	}

	int EngineContext::Terminate()
	{
		int r = 0;

		if (!MemorySubsystemIsActive())
			goto threading;
		r = TerminateMemorySubsystem() != 0;
	threading:
		if (!ThreadSubsystemIsActive())
			goto io;
		r = TerminateThreadSubsystem() != 0;
	io:
		if (!IOSubsystemIsActive())
			goto io;
		r = TerminateIOSubsystem() != 0;
	periph:
		if (!PeripheralSubsystemIsActive())
			goto video;
		r = TerminatePeripheralSubsystem() != 0;
	video:
		if (!VideoSubsystemIsActive())
			goto audio;
		r = TerminateVideoSubsystem() != 0;
	audio:
		if (!AudioSubsystemIsActive())
			goto returnblk;
		r = TerminateAudioSubsystem() != 0;
	returnblk:
		return r;
	}
}


