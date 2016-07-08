# ReprintMapper
## Version: 0.2.1

A programme to convert Copyfind outputs into a list of direct ancestor-descendent matches; part of the [Scissors and Paste Project]( https://osf.io/nm2rq/)

### Input

The ReprintMapper requires an input file in tab-separate value (.tsv) format. The file should have the data in the following columns, without a header.
+ Perfect_Match_Score
+ Left_Match_Score
+ Right_Match_Score
+ Target_Year
+ Target_Month	
+ Target_Day
+ Target_Title
+ Target_Identifier
+ Source_Year
+ Source _Month
+ Source _Day	
+ Source _Title 	
+ Source_ Identifier

Example inputs are available at [https://github.com/mhbeals/BL19thC_Reprints/tree/master/Formatted%20Matching%20Reports]( https://github.com/mhbeals/BL19thC_Reprints/tree/master/Formatted%20Matching%20Reports)

### Use

Core programme file (ReprintMapper_v_0_2_1.cpp) should be modified to indicate path to input file and desired output folder. 
Compile or run from debug window.
