# Test cases

Run the program from the project folder. "Keys" are what you type at the menu.

| # | Test | Keys | Expected result |
|---|---|---|---|
| 1 | Data loads | (start program) | `Loaded 15 products and 20 connections.` |
| 2 | View products | `1` | Table of 15 products, IDs 1 to 15 |
| 3 | BFS from Laptop | `3`, `1` | Wireless Mouse, Mechanical Keyboard, USB-C Hub, Laptop Backpack, 27-inch Monitor |
| 4 | DFS from Laptop | `4`, `1` | Wireless Mouse, Mechanical Keyboard, USB-C Hub, Laptop Backpack, Laptop Stand |
| 5 | Dijkstra from Laptop | `5`, `1` | Backpack (cost 2), Mouse (3), Laptop Stand (3), Keyboard (4), Monitor (4) |
| 6 | Dijkstra through middle products | `5`, `9` | Phone Case (1), Fast Charger (2), Wireless Earbuds (3), Headphones (9), Laptop (16) |
| 7 | Trending | `6` | Laptop 9 connections, then Keyboard, USB-C Hub, Smartphone, Laptop Stand with 3 each |
| 8 | Add product and link | `7`, `16`, `Power Bank`, `Accessories`, `1499`, then `8`, `16`, `9`, `7`, then `5`, `16` | `Product added.`, `Connection added.`, then Smartphone (4), Phone Case (5), Fast Charger (6), Wireless Earbuds (7), Headphones (13) |
| 9 | Unknown product ID | `3`, `99` | `No product with ID 99.` |
| 10 | Duplicate product ID | `7`, `1` | `A product with ID 1 already exists.` |
| 11 | Connect product to itself | `8`, `1`, `1`, `5` | `A product cannot be connected to itself.` |
| 12 | Connect unknown product | `8`, `1`, `99`, `5` | `One or both product IDs do not exist.` |
| 13 | Strength out of range | `8`, `1`, `2`, `11` | `Strength must be between 1 and 10.` |
| 14 | Duplicate connection | `8`, `1`, `2`, `5` | `These products are already connected.` |
| 15 | Non-number at menu | `abc` | `Please enter a whole number.` |
| 16 | Menu number out of range | `12` | `Invalid choice. Pick 0-8.` |
| 17 | Run from wrong folder | start from another folder | `Could not open data/products.txt. Run the program from the project folder.` |
| 18 | Bad lines in data files | add `16,Broken,Accessories,abc` to products.txt and `1,99,5` to edges.txt | `Skipped bad product line` and `Skipped invalid connection` messages, program still starts |
