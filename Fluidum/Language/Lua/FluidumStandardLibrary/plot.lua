FPlot = {

    --figure
	Make = function(title ,labelX ,labelY)

		--default
		if title == nil then
			title = "NoName"
		end
		if labelX == nil then
			labelX = "x"
		end
		if labelY == nil then
			labelY = "y"
		end
		
		--index
		local figureIndex<const> = _Internal_Plot_.Make(title,labelX,labelY)
			    
		
	    return --return table
	    {
			Display = function()
				_Internal_Plot_.Display(figureIndex)
			end,

			Hide = function()
				_Internal_Plot_.Hide(figureIndex)
			end,

			--plot
			MakePlot = function(valuesX ,valuesY)
			
				local plotIndex<const> = _Internal_Plot_.MakePlot(figureIndex ,valuesX ,valuesY)
			    
			    return --return table
			    {
					Plot = function(x ,y)    
						_Internal_Plot_.Plot(figureIndex ,plotIndex ,x ,y)
					end,

					SetMarker = function(marker)	
						_Internal_Plot_.SetMarker(figureIndex ,plotIndex, marker)
					end,

					PushBack = function(x ,y)
						_Internal_Plot_.PushBack(figureIndex ,plotIndex ,x ,y)
					end
			    }

			end,

	    
	    }

	end
}