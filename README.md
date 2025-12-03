# 🖼️ Steganography Project (C Programming – LSB Method)

This project implements **Image Steganography** using the **Least Significant Bit (LSB)** technique in the C programming language.
It hides a secret message inside a BMP image without visibly altering the original image.
The project supports both **encoding (hiding)** and **decoding (extracting)** operations.

---

## 📁 Project Structure

```
Steganography-Project/
│── beautiful.bmp        # Sample BMP image
│── default.bmp          # Output (stego) image
│── secret.txt           # Message to hide
│── output.txt           # Decoded output message
│── main.c               # Main controller file
│── encode.c             # Encoding (hide data) logic
│── encode.h             # Encode function declarations
│── decode.c             # Decoding (extract data) logic
│── decode.h             # Decode function declarations
│── common.h             # Common reusable definitions
│── types.h              # Custom data types used in project
```

---

## 🚀 Project Overview

This project hides a text message inside a BMP image by modifying the **least significant bit** of each pixel.
Since changing only the last bit does not affect the visual quality of the image, the resulting **stego-image** looks identical to the original.

You can then extract the hidden message from the stego-image using the decoding function.

---

## 🔧 How It Works (LSB Technique)

### **Encoding**

1. Read the BMP image pixel data.
2. Read the text from **secret.txt**.
3. Convert the text into binary.
4. Replace the LSB of each pixel with message bits.
5. Store the modified image as **default.bmp**.

### **Decoding**

1. Read the LSBs of pixels from **default.bmp**.
2. Reconstruct the bits into characters.
3. Write the decoded message to **output.txt**.

---

## ▶️ How to Compile and Run

### **Compile**

```bash
gcc main.c encode.c decode.c -o stego
```

### **Run Encoding**

```bash
./stego -e beautiful.bmp secret.txt default.bmp
```

### **Run Decoding**

```bash
./stego -d default.bmp output.txt
```

---

## 📝 Features

* Hide secret text inside BMP images
* Extract hidden text accurately
* Supports any .bmp 24-bit image
* Image quality remains unchanged
* Modular code design (encode/decode separated)
* Very fast and lightweight

---

## 🎯 Applications

* Secure communication
* Data hiding
* Digital watermarking
* Covert messaging in forensics
* Cybersecurity & privacy research

---

## 🔮 Future Enhancements

* Add encryption (AES/RSA) before embedding
* Add GUI for easier usage
* Extend support for audio/video steganography
* Add error detection & correction
* Implement advanced methods (DCT/DWT)

---

