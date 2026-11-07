# Model panel integration

User-approved scope: independently authored Astra, Sol and Terra popup cards, arranged left-to-right in equal screen thirds. Title, body and close button; translucent rounded background blur. Popups are non-modal and do not capture focus or block input elsewhere. Add a shared `모든 팝업 열기` button below the three openers. Work on main and make one final commit, no per-chat commits.

Ownership: master handles Astra, integration, tests and final commit; Sol and Terra chats handle their own runtime classes, editor generators and WBPs. UE execution is serialized through the master.

- [x] Switch main and fast-forward previously completed work, retaining existing changes.
- [x] Assign model-specific source ownership; remove focus traps and prevent duplicate opens.
- [x] Add shared open-all routing without runtime widget-tree construction.
- [x] Build all sources and run panel asset tests against old assets to establish failure.
- [x] Generate each model's saved WBPs under exclusive UE ownership.
- [x] Run automated checks and actual game checks: all-open, independent close/reopen, repeated opens, input across panels, blur and corners.
- [x] Commit the combined widget work once. Exclude pre-existing level and DefaultInput.ini changes.

Layout: opener anchors x=(index+.5)/3, y=0, offset y=80, 300x60. All-open at x=.5, y=160. Panel bounds index/3 to (index+1)/3 horizontally, .32 to .85 vertically, 20-unit side inset. Blur strength 8 and corner radius 20. Each model retains its own body and styling.

Validation: UMG57Editor Development build succeeded. Old WBPs failed NonModalPanels as expected; after regeneration, Automation RunTests UMG57. passed with exit 0. All three final generator runs exited 0 without errors/warnings. Widget GUID maps are synchronized before compile. Standalone game verified all-open, repeated all-open, each independent close/reopen, cross-panel button input, Tab navigation, visible rounded translucent blur cards, and no remaining duplicates after closing once per model. Runtime log has no Error/Ensure/Fatal entries. Independent source review found no actionable defects. Existing toolchain preference/include-order warnings and a nonfunctional Sol generator variable-shadow warning remain.
