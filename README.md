

# Karnaugh Map Simplifier in C++

## Overview

This project implements **Karnaugh Map (K-Map)** Boolean expression minimization using **two different techniques**:

1. **Greedy Grouping on a 4-variable K-Map**
2. **Quine-McCluskey Tabulation Method** for up to 10 variables

Both programs take **minterms** and **don't-care terms** as input and return the simplified Boolean expression in **Sum of Products (SOP)** form.

---

## 🔧 Files

- `greedy_kmap.cpp`:  
  Implements a greedy grouping approach for up to **4-variable** K-Maps.  
  It simulates manual K-Map simplification by detecting and grouping adjacent 1s or Xs (don't cares) using spatial folding logic.

- `quine_mccluskey.cpp`:  
  Implements the **Quine-McCluskey algorithm**, suitable for up to **10 variables**.  
  It performs tabulation-based minimization with binary string manipulation and essential prime implicant selection.

---

## How It Works

### 1. Greedy K-Map Grouping
- Converts given minterms into 2D folded grid using **Gray code** row/column mapping.
- Attempts to form maximal groups (1x4, 2x2, etc.) including don't-care terms.
- Generates expression terms using unchanged bits among grouped minterms.

### 2. Quine-McCluskey Method
- Converts all minterms and don't-cares into binary strings.
- Iteratively merges pairs of terms differing by one bit to form implicants.
- Identifies prime implicants and selects essential ones to cover all minterms.
- Final result is built from binary implicants translated into literal form (e.g., `A'B`).

---

##  Features

- Accepts minterms and optional don't-care terms
- Supports:
  - Up to **4 variables** using K-Map
  - Up to **10 variables** using Quine-McCluskey
-  Outputs simplified **SOP expression**
-  Validates input range and handles errors

---

## How to Run

Compile each file separately:

```bash
g++ final.cpp -o kmap_greedy
./kmap_greedy

g++ quine_final.cpp -o kmap_qm
./kmap_qm

