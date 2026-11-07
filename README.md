# UE5 UMG 모델별 구현 비교

UE 5.7 프로젝트에서 GPT-6 Astra, GPT-6.1 Sol, GPT-6.1 Terra가 작성한 UMG 팝업을 비교합니다.

- 프로젝트: [UMG57.uproject](UMG57/UMG57.uproject)
- **[시안 기반 팝업 6종 구현 비교 평가 — 실제 캡처·슬라이더 동작 검증](docs/reports/concept-popup-implementation-comparison.md)**
- **[모델별 팝업 구현 비교 분석 보고서 — 실제 실행 캡처 포함](docs/reports/model-popup-comparison.md)**
- **[모델별 팝업 디자인 이미지 비교 — 자유 디자인 및 수평 슬라이더 시안 6장](docs/reports/popup-design-concepts-comparison.md)**

기본 플레이에서 왼쪽부터 Astra·Sol·Terra의 기본형과 슬라이더형 버튼을 사용할 수 있습니다. 각 팝업은 화면의 고유 영역에 열리며, ‘모든 팝업 열기’ 또는 ‘모든 슬라이더 열기’로 세 모델을 함께 비교할 수 있습니다. 같은 열의 두 종류는 서로 교체되며 슬라이더 값은 세션 동안 유지됩니다.
