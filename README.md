# Unreal Engine 5.4 C++ AI Builder System

A comprehensive AI system for Unreal Engine 5.4 featuring modular behavior management, advanced perception, and flexible state machines.

## Features

### Core Components
- **AIBuilderCharacter**: Main AI character class with integrated perception and behavior systems
- **AIBuilderController**: Custom AI controller with enhanced perception management
- **AIBuilderStateMachine**: Flexible state machine for AI behavior management
- **AIBuilderSensorComponent**: Advanced multi-sensor detection system

### AI Capabilities
- Multi-layered perception system (sight, hearing, touch)
- Configurable behavior trees and blackboard integration
- Dynamic state transitions with validation
- Memory-based target tracking
- Performance-optimized sensor updates

### State Management
- Idle, Patrol, Chase, Attack, Search, and Return states
- Smooth state transitions with validation rules
- Event-driven state change notifications
- Configurable transition delays and conditions

## Installation

1. Copy the `AIBuilder` folder to your project's `Source` directory
2. Add `AIBuilder` to your project's `.uproject` file in the modules section:
   ```json
   {
     "Name": "AIBuilder",
     "Type": "Runtime",
     "LoadingPhase": "Default"
   }
   ```
3. Regenerate project files and compile

## Usage

### Basic Setup
1. Create a Blueprint based on `AIBuilderCharacter`
2. Assign a Behavior Tree and Blackboard Asset
3. Configure perception settings (sight range, angles, etc.)
4. Set up patrol points and behavior parameters

### Advanced Configuration
- Customize sensor ranges and detection parameters
- Create custom state transition rules
- Implement specific attack and movement behaviors
- Set up debug visualization for development

### Blueprint Integration
All components are fully Blueprint-compatible with exposed functions:
- `SetMovementSpeed()`
- `StartBehaviorTree()` / `StopBehaviorTree()`
- `GetCurrentTarget()` / `SetCurrentTarget()`
- State machine controls and sensor management

## Architecture

### Modular Design
Each component handles specific functionality:
- Character class manages overall AI coordination
- Controller handles perception and blackboard updates
- State machine manages behavior transitions
- Sensor component provides multi-modal detection

### Performance Considerations
- Configurable update frequencies for expensive operations
- Efficient memory pooling for detected actors
- Optimized line-of-sight checks with caching
- Event-driven updates to minimize unnecessary calculations

## Debugging

The system includes comprehensive logging through `LogAIBuilder` category:
- State transitions and behavior changes
- Perception events and target acquisition/loss
- Sensor detections and confidence levels
- Performance metrics and error handling

## Extending the System

### Custom States
Add new states to `EAIBuilderState` enum and implement corresponding update functions in the state machine.

### Additional Sensors
Extend `UAIBuilderSensorComponent` with new sensor types and detection methods.

### Behavior Integration
Create custom behavior tree tasks that interface with the state machine and sensor data.

## Requirements

- Unreal Engine 5.4+
- C++17 compatible compiler
- AI and Navigation modules enabled

This system provides a solid foundation for complex AI behaviors while maintaining performance and extensibility.

## Project Structure
The `AIBuilder` module contains the core C++ logic for AI-driven gameplay in the project. Below is the folder structure:

```
YourProject/
└── Source/
    └── AIBuilder/
        ├── AIBuilder.Build.cs
        ├── Public/
        │   ├── AIBuilder.h
        │   ├── AIBuilderController.h
        │   └── Core/
        │   │   └── AIBuilderCharacter.h
        │   └── Components/
        │       ├── AIBuilderStateMachine.h
        │       └── AIBuilderSensorComponent.h
        └── Private/
            ├── AIBuilder.cpp
            ├── AIBuilderController.cpp
            └── Core/
            │   └── AIBuilderCharacter.cpp
            └── Components/
                ├── AIBuilderStateMachine.cpp
                └── AIBuilderSensorComponent.cpp
