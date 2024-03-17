<img width="150" alt="CG-Spectrum-Icon_RGB squaresmall" src="https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/76f032ce-cb98-48ca-8b39-0e640e8cfcff">

# Project Pumpkin
This repository contains a game that was developed for 8 weeks long autumn-themed challenge from CG Spectrum. Our [team](#Team) of 5 decided to make a stylized top-down shooter that is enhanced by additional game mechanics, has some replayability and fun, comfort gameplay. Project Pumpkin features main menu and 1 game level, 7 unique models and other assets links to which can be found [here](Content/TopDown/UI/Widgets/CreditsUI.uasset).

# Technical overview
Written summary of the technologies and practices used to develop the code side of the game.

## How to use
Requires Unreal Engine 5.3.^ to build, edit and package the game.

## ECS
![ECS](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/edc904e9-03ee-4183-9ffc-d557aff4db0a)

>Related content: [MassHorde](https://github.com/Razdvizh/ProjectPumpkin/tree/main/Source/MassHorde), [DataAssets](https://github.com/Razdvizh/ProjectPumpkin/tree/main/Content/Horde/DataAssets)

Project Pumpkin uses new Unreal Engine 5 entity component system called `Mass` to spawn large quantities of enemies and process their AI. Data-oriented design focuses on performance by aiming to create programs that meet the best expectations of hardware internals. That's why it was chosen to create hordes of pumpkins, up to 1500 actors were present with decent performance both CPU and memory wise!

## Gameplay
![Gameplay](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/d123b086-5bf7-4078-aebc-398741dd6f46)

>Related content: [Character](https://github.com/Razdvizh/ProjectPumpkin/blob/main/Source/ProjectPumpkin/ProjectPumpkinCharacter.h) and a lot more!

Project Pumpkin's gameloop under the hood is classes, components, inheritance, polymorphism and interfaces all propagated to blueprints for quick iterations. While I and team was considering time limits, conclusion came that even game small as Project Pumpkin demands minimal architecture for its mechanics and fast, robust changes. Classes are fairly encapsulated and extendable, most of them also data-driven. 

In the end well configured by game designers good and bad pumpkins contribute to interesting gameplay while tested and calibrated input feels smooth and natural.

## User Interface
![UI](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/9dd4bb43-f0bb-45b5-84f3-1f7e51850a93)

>Related content: Widgets and [materials](https://github.com/Razdvizh/ProjectPumpkin/tree/main/Content/TopDown/UI/Assets/Materials)

Project Pumpkin is fully written in C++ including widgets. Main benefit is UI/UX design by artists with no worrying about logic and interactions with other blueprints. Some elements are rendered and controlled by through materials rather than regular progress bars. Materials help with customization and, while this is irrelevant for the Project Pumpkin, larger games may use this approach to delegate work from CPU to GPU, potentially increasing performance.

<a id="Team"></a>
# Team
see Contributors.

| Person        | Role          |
| ------------- |:-------------:|
| Razdvizh      | Programmer    |
| CBT94         | 3D Artist     |
| LibratVeritas | 3D Artist     |
| James Dameris | Game Designer |
| Kelly Harris  | Game Designer |
