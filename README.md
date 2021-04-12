Código utilizado para la implementación de los controladores.

Dentro de la carpeta src/:

* **motorPWM.cpp** -> Contiene las funciones que manejan el PWM HW
* **main.cpp** -> Contiene el setup y loop principal además de las configuración de las interrupciones necesarias (encoder y timers)

Dentro de la carpeta python/:

* **fromDatatoFile.py** -> Script para tratar los datos obtenidos de los experimentos y generar archivos para el script de MATLAB (obtener archivos .mean)
* **readSerial.py** -> Script que lee el puerto serie y almacena en el archivo que se indique
* **to_csv.py** -> Script que crea experimentos que serán leidos desde el Arduino
