# Task 5 – Contact Management System (C++)

CLI contact book with full CRUD, search and file persistence.

## Features
- `struct Contact` (name, phone, email, address) stored in a `std::vector`
- Add, view (sorted by name), search (name – case-insensitive – or phone), edit, delete
- Contacts are loaded from `contacts.txt` on startup and saved on every change and on exit
- Delete asks for confirmation; edit keeps old value when you press Enter

## File format
```
name|phone|email|address        (one contact per line)
```
A small sample `contacts.txt` is included. The "Contact Seed Data" from the portal can be used
by converting it to this pipe-delimited format.

## Build & Run
```bash
g++ -std=c++17 -Wall -Wextra contacts.cpp -o contacts
./contacts
```
Run it from this folder so it finds `contacts.txt`.

## Files
| File | Purpose |
|------|---------|
| `contacts.cpp` | Source code |
| `contacts.txt` | Sample data |
| `sample_output.txt` | Captured output |
