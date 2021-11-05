FPlot = {

    --Create
	Create = function(title ,labelX ,labelY)

		--Default
		if( type(title) == "nil")
			title = "NoName"
		if( type(labelX) == "nil")
			labelX = "x"
		if( type(labelY) == "nil")
			labelY = "y"
		
		local number<const> = FPlot.Create(title,labelX,labelY)
			    
		local plotIndex = 1

	    return --return table
	    {
			SetMarker = function(marker)	
				_Internal_Plot_.SetMarker(number ,plotIndex, marker)
			end,

			PushBack = function(x ,y)
				_Internal_Plot_.PushBack(number ,plotIndex ,x ,y)
			end,

			SetPlotIndex = function(index)
				plotIndex = index
			end,

			GetPlotIndex = function()
				return plotIndex
			end

	    
	    }

	end
}