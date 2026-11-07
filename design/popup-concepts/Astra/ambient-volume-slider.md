# Astra slider popup — Ambient volume

- Date: 2026-11-08 (Asia/Seoul)
- Prompt design: GPT-6 Astra / Medium
- Generation tool: built-in image_gen.imagegen
- Image generation model: undisclosed by tool
- Requested/actual size: 1536×1024 / 1536×1024 PNG
- Output: ambient-volume-slider.png
- Iterations: one generation, no image edits; independent second concept, no reference image.
- Concept: dark olive/charcoal audio settings with a mint horizontal volume slider and 65% value.
- Inspection: complete popup, readable title/body, horizontal fill/remaining track/round thumb, value and endpoints clearly visible. Thumb is visually near the indicated value; no exact numerical mapping or functional interaction is established by this raster image. Close versus Done semantics would need definition in implementation.

## Final prompt

```text
Use case: ui-mockup.
Generate a finished original popup UI design image on a 1536x1024 landscape canvas. One front-facing interface, not a physical device photograph. Functional concept: adjust ambient sound volume for a focused listening session. Make the horizontal slider the central interaction, unmistakable and visually accurate.
Art direction: a calm professional audio workspace with charcoal and smoky olive tones, pale mint accents, warm off-white typography. Center a substantial rounded charcoal popup over a dim out-of-focus audio workspace background. Generous inset padding, precise typographic alignment, subtle hairline borders, restrained depth, no neon or busy decoration. Use modern sans-serif typography throughout. A tiny speaker icon beside the eyebrow, an X close affordance in the top right, an extremely subtle waveform motif only in the header.
Exact visible text:
eyebrow "SOUND SETTINGS"
title "Find your quiet."
body "Set the background sound to a level that helps you focus."
slider label "Ambient volume"
large current value "65%"
end labels "Silent" and "Full"
supporting text "Changes are previewed as you adjust."
bottom secondary button "Close"
bottom primary button "Done"
Layout: Title and body at top left. In a gently inset control area, label left and large value 65% right on the same row. One long, straight horizontal slider below. Pale mint filled track from the left to EXACTLY 65 percent of track length, charcoal-gray remaining track, clearly visible circular mint thumb centered at the 65 percent boundary. Left/right end labels beneath the respective ends. Give the slider a generous vertical interaction zone and keep it distinct from decoration. Preview note below. Bottom action row separated by a thin line, Close outlined and Done solid mint with charcoal text, both at bottom right. One popup only with complete visible rounded boundary and comfortably readable text. No extra controls, no logos, watermarks, model names, devices, hands or explanatory arrows. Prioritize an interface that could actually be implemented over ornamental illustration.
```
