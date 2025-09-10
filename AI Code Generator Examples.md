# AI Code Generator Examples

## How to Use the AI Code Generator

The AI Code Generator can understand natural language requests and generate C++ code for Unreal Engine. Here are examples of what you can ask for:

### Character Creation Examples

**Request:** "Create a guard character that patrols between waypoints and chases enemies when detected"
- Generates: AIGuardCharacter class with patrol and chase behaviors
- Includes: Perception components, behavior tree integration, target detection

**Request:** "Make an AI character that follows the player and attacks nearby enemies"
- Generates: AIFollowerCharacter class with follow and combat behaviors
- Includes: Player tracking, enemy detection, attack systems

**Request:** "Create a flying AI character that scouts areas and reports back"
- Generates: AIScoutCharacter class with flying movement and communication
- Includes: Flying movement component, area scanning, reporting system

### Behavior Tree Task Examples

**Request:** "Create a behavior tree task that makes the AI search for cover when health is low"
- Generates: BTTask_FindCover class
- Includes: Health checking, cover point detection, movement to cover

**Request:** "Make a task that causes the AI to investigate sounds"
- Generates: BTTask_InvestigateSound class
- Includes: Sound location tracking, investigation behavior, timeout handling

**Request:** "Create a task for collecting items in the environment"
- Generates: BTTask_CollectItems class
- Includes: Item detection, pathfinding, inventory management

### AI Controller Examples

**Request:** "Create an AI controller with advanced perception and decision making"
- Generates: AdvancedAIController class
- Includes: Multi-sense perception, decision trees, behavior coordination

**Request:** "Make a controller that manages squad-based AI behavior"
- Generates: SquadAIController class
- Includes: Squad communication, formation management, coordinated attacks

### Component Examples

**Request:** "Create a component that handles AI communication and alerts"
- Generates: AICommunicationComponent class
- Includes: Message passing, alert systems, group coordination

**Request:** "Make a component for AI inventory and item management"
- Generates: AIInventoryComponent class
- Includes: Item storage, usage decisions, resource management

## Advanced Features

### Natural Language Processing
The system can understand:
- Action words: "patrol", "chase", "attack", "follow", "guard"
- Object types: "character", "controller", "task", "component"
- Behaviors: "when health is low", "detect enemies", "find cover"
- Conditions: "if player nearby", "when attacked", "after delay"

### Code Generation Features
- Automatic include generation
- Blueprint integration
- Proper UE5 coding standards
- Memory management
- Error handling
- Performance optimization

### Integration Examples

```cpp
// In your game code, you can use the generator like this:
UAICodeGenerator* Generator = FAIBuilderModule::GetCodeGenerator();
FGeneratedCode Code = Generator->CreateAICharacter("MyGuard", "Patrol and defend area");

// Save the generated code
Generator->SaveGeneratedCode(Code, "Source/MyProject/AI/");
```

### Blueprint Integration
The system includes a UMG widget that provides a user-friendly interface:
- Text input for natural language requests
- Real-time code preview
- One-click code generation and saving
- Example request templates

## Tips for Better Results

1. **Be Specific**: "Create a guard that patrols 3 waypoints" vs "Create a guard"
2. **Include Behaviors**: Mention what the AI should do in different situations
3. **Specify Components**: Mention if you need perception, movement, combat, etc.
4. **Use Keywords**: Words like "patrol", "chase", "attack" trigger specific templates

## Extending the System

You can extend the AI Code Generator by:
- Adding new templates in `InitializeTemplates()`
- Adding keywords in `InitializeAIKeywords()`
- Creating custom parsing rules in `ParseUserRequest()`
- Adding new generation functions for specific AI types
