# Six concept popup implementation plan

- Work on main. Astra, Sol, Terra own their runtime classes and editor authoring helpers; the master owns integration, tests and reporting.
- Implement each model's saved basic and horizontal-slider concept as editable Widget Blueprints. Never build widget trees at runtime.
- Preserve model columns and outside input. Each column shows one variant at a time; six named openers plus shared basic/slider controls.
- Persist live slider values in the model main widget for the session. Preview only local UI effects; do not claim real sound, lighting or weather integration.
- Build all source together, generate the three asset sets serially, run automation, then exercise real game input and capture both variants.
- Compare design fidelity, typography/layout, actual behavior, lifecycle and limitations in a README-linked report. Do not stage or commit during delegated work.
