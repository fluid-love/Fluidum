local _Internal_Array_ function {
	local FluidumArray<const> = _Fluidum_Register_CppFunctions_.Array()

	make = function(value_type)
		collectgarbage()
		local key<const>, type = FluidumArray.make(value_type);--key(std::vector indedx)

		if type == "n" then --number
			return {
				--member functions
				front = function()
					FludiumArray.front_number(key)
				end,

				back = function()

				end
			}
		else if type == "s" then --string

		else if type == "b" then --bit
		
		else 
			error("Internal Error.")
		end
	end
}
return _Internal_Array_()