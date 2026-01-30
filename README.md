🎨 Computer Graphics Algorithms in Turbo C++
A comprehensive implementation of fundamental graphics algorithms using Turbo C++ with BGI graphics

📋 Table of Contents
✨ Features

🧮 Algorithms Implemented

🚀 Quick Start

🖥️ How to Run

🎮 Program Controls

📁 Project Structure

🔧 Technical Details

📸 Sample Output

🤝 Contributing

📄 License

👨‍💻 Author

✨ Features
✅ Multiple Algorithms: DDA, Bresenham's, Midpoint algorithms for lines and circles

✅ Professional UI: Clean graphical interface with navigation menus

✅ Interactive Input: User-friendly input system with validation

✅ Coordinate System: Visual grid with coordinate axes

✅ Real-time Display: Shows algorithm parameters during drawing

✅ Point Marking: Clearly marks endpoints, centers, and vertices

✅ Keyboard Navigation: Arrow key support with ESC for navigation

🧮 Algorithms Implemented
Line Drawing
DDA Algorithm (Digital Differential Analyzer)

Bresenham's Line Algorithm

Circle Drawing
Midpoint Circle Algorithm

Bresenham's Circle Algorithm

Other Shapes
Ellipse Drawing Algorithm

Rectangle Drawing

Triangle Drawing

🚀 Quick Start
Prerequisites
Turbo C++ IDE installed

BGI graphics files available

Setup
Clone the repository:

bash
git clone https://github.com/Mehar59/ComputerGraphics.git
cd ComputerGraphics
Open graphics_project.cpp in Turbo C++

Ensure BGI files are accessible:

Default path: C:\TURBOC3\BGI\

Or update the path in initgraph() function

Compile and Run:

Compile: Alt + F9

Run: Ctrl + F9

🖥️ How to Run
Launch Program: Run the compiled executable

Main Menu: Select from 6 options using number keys

Sub-menus: Choose algorithms (where applicable)

Input Coordinates: Type values when prompted

View Results: See the drawn shapes with coordinate grid

Navigate: Press any key to return to menu

🎮 Program Controls
Key	Action
1-6	Select main menu options
Arrow Keys	Navigate sub-menus
ENTER	Confirm selection
ESC	Cancel/go back
BACKSPACE	Delete input characters
Number Keys	Enter coordinates
📁 Project Structure
text
ComputerGraphics/
├── graphics_project.cpp    # Main source code
├── README.md              # This documentation
├── screenshots/           # Project screenshots
│   ├── main_menu.png
│   ├── line_demo.png
│   └── circle_demo.png
└── docs/                  # Additional documentation
🔧 Technical Details
Language: C++ (Turbo C++ compatible)

Graphics Library: BGI (Borland Graphics Interface)

Screen Resolution: 640×480 pixels

Color Palette: 16 colors

Input Method: Keyboard-based interactive input

Coordinate System: Cartesian grid with origin at center

Key Functions
cpp
dda_line()         // DDA line drawing algorithm
bresenham_line()   // Bresenham's line algorithm
midpoint_circle()  // Midpoint circle algorithm
bresenham_circle() // Bresenham's circle algorithm
draw_ellipse()     // Ellipse drawing algorithm
drawCoordinateSystem() // Draws grid and axes
get_number()       // Handles user input with validation
📸 Sample Output
text
MAIN MENU:
╔══════════════════════════════╗
║      GRAPHICS DEMO           ║
╠══════════════════════════════╣
║ 1. Line Drawing Algorithms   ║
║ 2. Circle Drawing Algorithms ║
║ 3. Ellipse Drawing           ║
║ 4. Rectangle Drawing         ║
║ 5. Triangle Drawing          ║
║ 6. Exit Program              ║
╚══════════════════════════════╝
Select option (1-6): _
🤝 Contributing
Contributions are welcome! To contribute:

Fork the repository

Create a feature branch (git checkout -b feature/AmazingFeature)

Commit your changes (git commit -m 'Add AmazingFeature')

Push to the branch (git push origin feature/AmazingFeature)

Open a Pull Request

Development Guidelines
Follow existing code style

Add comments for complex algorithms

Test changes in Turbo C++ environment

Update documentation accordingly

📄 License
This project is licensed under the MIT License - see the LICENSE file for details.

text
MIT License

Copyright (c) 2024 [Your Name]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
👨‍💻 Author
Your Name
Computer Science Student | Graphics Programming Enthusiast

GitHub: @Mehar59

LinkedIn: Your Profile

Email: mehar2004sharma@gmail.com

Acknowledgments
Turbo C++ Community for preserving legacy tools

Computer Graphics educators for foundational knowledge

Open source community for inspiration

<div align="center">
⭐ If you find this project useful, please give it a star!
Made with ❤️ using Turbo C++
Last Updated: December 2024

</div>
