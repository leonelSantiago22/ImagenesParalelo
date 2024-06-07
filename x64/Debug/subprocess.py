import subprocess

# Nombre del archivo exe
exe_file = "BMP24bitsW.exe"

# Nombre del archivo de salida
output_file = "salidas.txt"

# Abrir el archivo de salida en modo escritura
with open(output_file, "w") as file:
    # Ejecutar el .exe 15 veces
    for i in range(15):
        try:
            # Ejecutar el .exe y capturar la salida
            result = subprocess.run([exe_file], capture_output=True, text=True)
            
            # Escribir la salida en el archivo
            file.write(f"--- Ejecución {i + 1} ---\n")
            file.write(result.stdout)
            file.write(result.stderr)
            file.write("\n\n")
            
        except Exception as e:
            # Capturar cualquier error durante la ejecución del .exe
            file.write(f"--- Ejecución {i + 1} ---\n")
            file.write(f"Error: {e}\n")
            file.write("\n\n")

print(f"Las salidas han sido guardadas en '{output_file}'.")
