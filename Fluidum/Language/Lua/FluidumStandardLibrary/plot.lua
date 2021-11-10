FPlot = {

    --figure
	Make = function(title ,labelX ,labelY)

		--default
		if type(title) == "nil" then
			title = "NoName"
		end
		if type(labelX) == "nil" then
			labelX = "x"
		end
		if type(labelY) == "nil" then
			labelY = "y"
		end
		
		--identify
		local number<const> = _Internal_Plot_.Make(title,labelX,labelY)
			    
		local plotIndex = 1

		
	    return --return table
	    {
			--plot
			MakePlot = function(valuesX ,valuesY)
			
				local plotNumber<const> = _Internal_Plot_.MakePlot(number)

			    if type(valuesX) ~= "nil" then
			    	_Internal_Plot_.Plot(valuesX ,valuesY)
			    end
			    
			    	
			    return --return table
			    {
			        
			    }
			end,

			SetMarker = function(marker)	
				_Internal_Plot_.SetMarker(number ,plotIndex, marker)
			end,

			PushBack = function(x ,y)
				_Internal_Plot_.PushBack(number ,plotIndex ,x ,y)
			end
	    
	    }

	end
}