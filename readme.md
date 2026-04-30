# 💊 Pharmacy Dispense System (SE Lab ESE)

## 👤 Student Details
- **Name:** Neev Jain  
- **Roll No:** 241070028  
- **Course:** Software Engineering Lab (End Semester Exam)

---

# 🧠 Project Overview

This project implements a **Pharmacy Dispense System** in C++ along with **comprehensive testing**.

It includes:
- ✔ Menu-driven CLI system  
- ✔ Black Box Testing (15 Test Cases)  
- ✔ White Box Testing (CFG, Cyclomatic Complexity, Paths)  
- ✔ Automated Test Execution  
- ✔ Clean structured output  

The system validates:
- Drug availability  
- Expiry date  
- Quantity correctness  
- Allergy & drug interaction  

---

# ⚙️ System Features

## 🔹 Core Functionality
- Dispense medicine  
- Check inventory (stock & expiry)  
- Generate billing  

## 🔹 Testing Support
- Run all test cases automatically  
- Structured PASS/FAIL reporting  
- Output validation using captured streams  

---

# 🧪 Black Box Testing

Designed using:
- **Equivalence Class Partitioning (ECP)**  
- **Boundary Value Analysis (BVA)**  

## 📋 Test Cases Summary

| TC ID | Scenario | Input | Expected Output |
|------|--------|------|----------------|
| TC1 | Valid case | Valid stock, qty | Medicine dispensed |
| TC2 | Not logged in | No login | Access denied |
| TC3 | Invalid drug | Unknown drug | Error message |
| TC4 | Exact stock | stock=5, qty=5 | Stock becomes 0 |
| TC5 | Qty > stock | stock=5, qty=6 | Insufficient stock |
| TC6 | Zero stock | stock=0 | Out of stock |
| TC7 | Negative qty | qty=-2 | Invalid input |
| TC8 | Zero qty | qty=0 | Invalid input |
| TC9 | Expired drug | expiry < date | Blocked |
| TC10 | Expiry today | expiry = today | Allowed |
| TC11 | Allergy case | allergy=yes | Warning |
| TC12 | Interaction | interaction=yes | Alert |
| TC13 | Mixed case | valid + invalid | Partial handling |
| TC14 | Large qty | stock=1000 | Success |
| TC15 | Billing | valid case | Correct bill |

---

# 🧪 White Box Testing

## 🔹 Cyclomatic Complexity

Formula:
V(G) = E - N + 2P
Values:
- Nodes (N) = 19  
- Edges (E) = 23  
- Components (P) = 1
V(G) = 6

👉 **Cyclomatic Complexity = 6**  
👉 **6 Independent Paths required**

---

## 🔹 Independent Paths

| Path | Description | Covered Test Cases |
|------|------------|-------------------|
| P1 | Invalid quantity | TC7, TC8 |
| P2 | Drug not found | TC3 |
| P3 | Expired drug | TC9 |
| P4 | Stock error | TC5, TC6 |
| P5 | Valid flow | TC1, TC4, TC14, TC15 |
| P6 | Valid + warnings | TC11, TC12 |

---

## 🔹 White Box Results

| Path | Result |
|------|--------|
| P1 | PASS |
| P2 | PASS |
| P3 | PASS |
| P4 | PASS |
| P5 | PASS |
| P6 | PASS |

---

# 📊 Coverage

- ✔ **Statement Coverage:** 100%  
- ✔ **Branch Coverage:** 100%  
- ✔ **Path Coverage:** 100%  

---

# ▶️ How to Run

## 🔧 Compile
```bash
g++ program.cpp -o app
1. Run System Manually (Manual Testing)
2. Run All Test Cases
3. Exit
TC1 : Valid case
Input: Logged in | Drug=Paracetamol | stock=10 | qty=5
Expected: Medicine dispensed successfully
Actual: Medicine dispensed successfully | Remaining stock: 5 | Bill: Rs. 50
Result: PASS
