Development
---
- **4/21/20** 
   - Created the JSON file containing state & borders info
   - Changed file names to make them more relevant to the project
   
- **4/22/20** 
   - Added the nlohmann/json external library
   - Updated CMakeLists.txt
   - Test cases for JSON deserialization

- **4/23/20** 
   - Downloaded an image of a map from Google Images
   - Used the Image library from cinder to display the image
     - Future: Figure out how to ask for input and get logic to work
     
- **4/29/20** 
   - Tried std::cin and cinder/TextBox for user input
   - Updated JSON file to include list that maps state to index
   
- **4/30/20** 
   - PrintText() to display text in window
   - Worked on game logic
     - Functional on its on without user input
   - Created helper functions to read input
     - Future: User input & show start/ending states in console or window
     
- **5/4/20** 
   - Updated JSON file for easier access to state index numbers
   - Figured out how to display text on screen
   - Connected user input to game logic
   - Added PROPOSAL.md file
   - Test cases for helper methods
   
- **5/5/20** 
   - Updated DEVELOPMENT.md file
   - Fixed bugs
     - Checked to see if string was empty before executing backspace function
     - Trimmed leading and trailing whitespace from user input