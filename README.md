# 🌲 ForestGuard  
### Giving Forests a Voice through Real-Time Acoustic Monitoring

ForestGuard is a **low-cost, autonomous, acoustic monitoring system** designed to detect and localize illegal and dangerous activities in forests — such as illegal logging, gunshots, or tree falls — in **real time**.

The project was developed and iterated during the **4th EELISA Prototype Contest (December 2025, Nürnberg, Germany)** using **user-centered design**, **co-creation**, and **Double (Blue) Diamond methodology**.

---

## 📌 Problem Statement

Forests cover more than **4.1 billion hectares globally**, yet most illegal activities inside them are detected **too late**.

### Key challenges identified through user research:
- Forests are too large to patrol manually  
- Rangers and NGOs are understaffed and overworked  
- Existing solutions (cameras, drones, satellites):
  - fail in dense forests
  - are expensive
  - detect damage only after it happens  
- Lack of **real-time alerts** and **precise localization**

👉 The core problem is **late detection**, not lack of enforcement.

---

## 💡 Solution Overview

ForestGuard enables forests to **signal danger instantly** by listening for abnormal acoustic events and sending alerts with precise location data.

### What ForestGuard does:
- Listens continuously to the forest environment  
- Detects specific sound patterns associated with threats  
- Calculates the location of the event using multiple nodes  
- Sends real-time alerts to rangers or monitoring systems  

---

## 🔬 Key Features

### 🎧 Acoustic Detection
- Detection of:
  - chainsaws
  - gunshots
  - tree falls
  - abnormal disturbances
- Uses **FFT-based feature extraction** and sound classification

### 📍 Localization (TDOA)
- Multi-node sound triangulation
- Time Difference of Arrival (TDOA)
- Newton–Raphson numerical solver
- Outputs actionable coordinates (not just alerts)

### ☀️ Autonomous Operation
- Solar-powered
- Low-energy microcontroller-based design
- Designed for long-term deployment
- No cables, no infrastructure

### 🔐 Privacy-Safe by Design
- No cameras
- No speech recognition
- Only non-identifiable acoustic features
- GDPR-friendly approach

---

## 🧠 Design Methodology

ForestGuard was developed using a **user-centered innovation process** based on the **Double (Blue) Diamond framework**:

### 1️⃣ Discover
- Interviews with:
  - forest rangers
  - NGO workers
  - environmental professionals
  - general public
- Identification of pain points:
  - late detection
  - lack of localization
  - high costs
  - limited manpower

### 2️⃣ Define
- Clear problem statement formulation
- Creation of:
  - personas
  - empathy maps
  - stakeholder maps
- Prioritization of assumptions and risks

### 3️⃣ Develop
- Ideation of multiple technical approaches
- Selection of acoustic monitoring as most viable
- Early technical validation of detection and localization

### 4️⃣ Deliver
- Prototype development
- Public testing at JOSEPHS Co-Creation Marketplace
- Live user feedback and validation
- Pitching to jury and stakeholders

---

## 🧪 Hypotheses Tested

### Hypothesis 1 — Detection Accuracy
**We believe that** dangerous sounds can be reliably detected with low false positives.  
**Validated through:** sound analysis tests and user trust feedback.

### Hypothesis 2 — Localization is Essential
**We believe that** alerts without precise location are not actionable.  
**Validated through:** interviews with rangers and NGO staff.

### Hypothesis 3 — Cost Determines Adoption
**We believe that** adoption requires a per-node cost below €100.  
**Validated through:** discussions with NGOs and public-sector stakeholders.

---

## 🌍 Market & Impact

### Target Users
- Forestry agencies
- National parks
- Environmental NGOs
- Anti-poaching units
- Biodiversity protection programs
- Carbon credit and sustainability projects

### Why ForestGuard Matters
- Prevents irreversible environmental damage
- Enables faster response times
- Scales across continents
- Supports EU Green Deal & sustainability regulations
- Strong alignment with UN SDGs (13, 15)

---

## 🏆 Validation & Results

- Publicly tested during a **4-day open co-creation event** in Nürnberg
- Direct interaction with dozens of visitors and stakeholders
- **Highest public funding allocation** among all prototypes
- Strong validation of problem relevance and solution clarity

---

## 🛠️ Current Project Status

ForestGuard is currently at **prototype / proof-of-concept stage**.

### Implemented:
- Acoustic feature extraction (FFT)
- Event detection logic
- Localization concept and simulation
- System architecture design
- User validation & market research

### Planned Next Steps:
- Field testing in real forest environments
- Improved sound classification models
- Hardware optimization
- Network communication (LoRa / mesh)
- Mobile/web dashboard for alerts


---

## 🤝 Acknowledgements

This project was developed during the **EELISA x JOSEPHS: Building Market-Ready Innovations** program.

Special thanks to:
- **Dr. David Schkade** — for the invitation and support  
- **Prof. Dr. Claudia Leemann** — for daily co-creation workshops, user-centered thinking, and introducing the research map and Double (Blue) Diamond methodology  
- **Sonja Vögeler** — for being the heart of the event and creating a welcoming environment at JOSEPHS  
- **Johannes** — for team collaboration and introducing Nürnberg  
- The teams at **JOSEPHS GmbH**, **FAU Erlangen-Nürnberg**, and **EELISA**

---

## 👤 Author

**Kaan Nasurla**  
Student — Computer Science & Information Technology  
Faculty of Automatic Control and Computer Science
university POLITEHNICA of Bucharest

---

## 📜 License

This project is currently shared for **research and educational purposes**.  
License details will be added as the project evolves.

---

🌱 *With ForestGuard, forests can finally speak.*
