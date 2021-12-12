local _Internal_Array_ = function() 
	local FluidumArray<const> = _Fluidum_Register_CppFunctions_.Array()
	return {
		make = function(value_type)

			local key<const>, type = FluidumArray.make(value_type);--key(std::vector index)

			if type == "n" then --number
				local array_number_table = {
					--functions
					empty = function()
						return FluidumArray.empty_number(key)
					end,

					size = function()
						return FluidumArray.size_number(key)
					end,

					front = function(val)
						return FluidumArray.front_number(key, val)
					end,

					back = function(val)
						return FluidumArray.back_number(key, val)
					end,

					set = function(...)
						return FluidumArray.set_number(key, ...)
					end,

					push_front = function(...)
						FluidumArray.push_front_number(key, ...)
					end,

					push_back = function(...)
						FluidumArray.push_back_number(key, ...)
					end,

					pop_front = function()
						FluidumArray.pop_front_number(key)
					end,

					pop_back = function()
						FluidumArray.pop_back_number(key)
					end,

					clear = function()
						FluidumArray.clear_number(key)
					end,

					erase = function(index, range)
						FluidumArray.erase_number(key, index, range)
					end,

					erase_if = function()
						--FluidumArray.pop_back_number(key, ...)
					end

				}
				local array_metatable = {
					__metatable = false,
					
					__index = function(table, index)
						return FluidumArray.__index_number(key, index)
					end,

					__newindex = function(table, index, val)
						FluidumArray.__newindex_number(key, index, val)
					end,

					__gc = function()
						FluidumArray.__gc(key)
					end
				}
				setmetatable(array_number_table, array_metatable)
				return array_number_table

			--[[else if type == "s" then --string
				return {
				
				}
			else if type == "b" then --bit
				return {
				
				}]]
			else 
				error("Internal Error.")
			end
		end
	}
end
return _Internal_Array_()