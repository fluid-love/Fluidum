FSystem = {

	Terminate = function()
	    _Internal_System.Terminate()
	end,

	SleepSeconds = function(seconds)
		_Internal_System.SleepSeconds(seconds)
	end,

	SleepMilliSeconds = function(milliseconds)
		_Internal_System.SleepMilliSeconds(milliseconds)
	end
}