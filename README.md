<img width="150" alt="CG-Spectrum-Icon_RGB squaresmall" src="https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/76f032ce-cb98-48ca-8b39-0e640e8cfcff">

# Project Pumpkin
This repository contains a game that was developed for 8 weeks long autumn-themed challenge from CG Spectrum. Our [team](#Team) of 5 decided to make a stylized top-down shooter that is enhanced by additional game mechanics, has some replayability and fun, comfort gameplay. Project Pumpkin features main menu and 1 game level, 7 unique models and other assets links to which can be found [here](Content/TopDown/UI/Widgets/CreditsUI.uasset).

![Gameplay](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/b1d29c0a-bf20-488d-8a3b-54c5c4533b71)

# Technical overview
Written summary of the technologies and practices used to develop the code side of the game.

## How to use
Requires Unreal Engine 5.3.^ to build, edit and package the game.

## ECS
![ECS](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/28df02a6-9b8e-4a0a-8251-46db88028660)

>Related content: [MassHorde](https://github.com/Razdvizh/ProjectPumpkin/tree/main/Source/MassHorde), [DataAssets](https://github.com/Razdvizh/ProjectPumpkin/tree/main/Content/Horde/DataAssets)

Project Pumpkin uses new Unreal Engine 5 entity component system called `Mass` to spawn large quantities of enemies and process their AI. Data-oriented design focuses on performance by aiming to create programs that meet the best expectations of hardware internals. That's why it was chosen to create hordes of pumpkins, up to 1500 actors were present with decent performance both CPU and memory wise!

## Gameplay
![Developing](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/b75d42c4-9c91-4bba-84c8-786e6555ff82)

>Related content: [Character](https://github.com/Razdvizh/ProjectPumpkin/blob/main/Source/ProjectPumpkin/ProjectPumpkinCharacter.h) and a lot more!

Project Pumpkin's gameloop under the hood is classes, components, inheritance, polymorphism and interfaces all propagated to blueprints for quick iterations. While I and team was considering time limits, conclusion came that even game small as Project Pumpkin demands minimal architecture for its mechanics and fast, robust changes. Classes are fairly encapsulated and extendable, most of them also data-driven.

Other part is simple vector math and kinematics to make objects behave natural and fun for the player and predictable for the game designer.

In the end game designers have wide options for configuring gameplay while code is tangible enough to reflect them and can be refactored or expanded relatively quicky when necessary.

## User Interface
![UI](https://github.com/Razdvizh/ProjectPumpkin/assets/114196034/d8366128-6f51-487f-96de-0fa6cac29f69)

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
