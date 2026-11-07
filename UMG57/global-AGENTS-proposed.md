# Global Unreal Engine widget rules

- Put widget classes under an intermediate directory named for the actual working model (for example `Terra`). Record the actual model name and reasoning effort inside the source code; do not guess either value.
- Name widget classes using only their actual purpose and model name. Do not include the project name or reasoning effort in class names.
- Append the model name to Widget Blueprint names derived from classes, for example `_GPT61Terra`.
- Never construct the widget tree inside a widget class. Build and save the tree in the Widget Blueprint so it is visible and editable in the Unreal Editor.
- Never create a git commit unless the user explicitly requests a commit in the current instruction or separately instructs you to commit.
