# This script creates individual script files for 
# running formatted match reports through the 
# reprintmapper and a batch file for running all 
# the scripts in sequence

start_year = 1800
end_year = 1900
counter = 1
countery = 0
year = 0

# While we are before the final year
while year < end_year:
    
    # Set to new year
    year = start_year + countery
    
    # While we are within a single year
    while  counter <= 12:
        
        monthstring = "0" + str(counter)
        date = str(year) + "_" + str(monthstring[-2:])

        # Start file text
        
        string = "c:\\formatted_inputs\\"
        string += date
        string += ".tsv"
        string += "\n"
        string += "c:\\ReprintMapper\\Release\\"
        string += date
        string += ".tsv"
        
        # Write script file
        filename = "script_" + date + ".txt"
        text_file = open(filename, "w")
        text_file.write(string)
        text_file.close()
        
        # Move onto next month counter
        counter += 1
        
        # Continue month loop
        
    # Reset month counter  
    counter = 1
    
    # Update year counter
    countery += 1
    
    # Continue year loop 

counter = 1
countery = 0
year = 0
string = ""

# While we are before the final year
while year < end_year:
    
    # Set to new year
    year = start_year + countery
    
    # While we are within a single year
    while  counter <= 12:
        
        monthstring = "0" + str(counter)
        date = str(year) + "_" + str(monthstring[-2:])

        # Start file text
        
        string += "Reprintmapper_v_1_0_0.exe < script_" + date + ".txt\n"
            
        # Move onto next month counter
        counter += 1
        
        # Continue month loop
        
    # Reset month counter  
    counter = 1
    
    # Update year counter
    countery += 1
    
    # Continue year loop 

# Write script file
filename = "reprintmapper.bat"
text_file = open(filename, "w")
text_file.write(string)
text_file.close()