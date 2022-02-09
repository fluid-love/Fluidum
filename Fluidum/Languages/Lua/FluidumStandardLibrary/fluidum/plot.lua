_Internal_Plot_ = function()
	
	return {--return table

		--figure
		make = function(title ,labelX ,labelY)

			--default
			if title == nil then
				title = "Title"
			end
			if labelX == nil then
				labelX = "x"
			end
			if labelY == nil then
				labelY = "y"
			end
		
			--index
			local figureIndex<const> = _Internal_Plot_.make(title,labelX,labelY)
			    
		
			return --return table
			{
				display = function()
					_Internal_Plot_.display(figureIndex)
				end,

				hide = function()
					_Internal_Plot_.hide(figureIndex)
				end,

				--plot
				make_plot = function(valuesX ,valuesY)
			
					local plotIndex<const> = _Internal_Plot_.make_plot(figureIndex ,valuesX ,valuesY)
			    
					return --return table
					{
						plot = function(x ,y)    
							_Internal_Plot_.plot(figureIndex ,plotIndex ,x ,y)
						end,

						marker = function(marker)	
							_Internal_Plot_.SetMarker(figureIndex ,plotIndex, marker)
						end,

						push_back = function(x ,y)
							_Internal_Plot_.puah_back(figureIndex ,plotIndex ,x ,y)
						end
					}

				end,

	    
			}

		end

	}
end

return _Internal_Plot_()

return {


}