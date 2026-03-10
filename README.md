# Electricity Billing & Appliance Energy Tracker (C++)

This is a simple C++ console program that helps users record household appliances, calculate daily electricity usage, and estimate monthly electricity bills. The program also saves appliance data and billing summaries to text files.

## Features

- Add appliances (name, watts, hours used per day)
- View all saved appliances
- Search appliances by name
- Calculate total daily energy consumption (kWh)
- Estimate daily and monthly electricity cost
- Save appliance data to a file
- Store billing summaries in a report file
- Input validation for incorrect entries

## How the Calculation Works

Energy consumption per appliance:

kWh per day = (Watts / 1000) × Hours per day

Billing calculation:

Daily Cost = Total Daily kWh × Tariff  
Monthly Cost = Daily Cost × 30

## Files Used

- appliances.txt → stores appliance records
- billing_summary.txt → stores billing history

Appliance data is saved in this format:

Name|Watts|Hours

Example:

Television|120|5  
Fan|75|8  
Refrigerator|300|24

## How to Compile and Run

Using g++:

g++ main.cpp -o app  
./app

On Windows:

g++ main.cpp -o app.exe  
app.exe

## Menu Options

1. Register appliance
2. View appliances
3. Search appliance
4. Billing
5. Save to file
6. Exit

## Notes

- Watts must be greater than 0
- Hours must be between 0 and 24
- Appliance name cannot be empty
- Data is automatically saved after adding an appliance

This project is for learning purposes and demonstrates file handling, structures, arrays, and basic calculations in C++.
