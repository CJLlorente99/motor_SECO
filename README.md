Código utilizado para la implementación de los controladores.

Dentro de la carpeta src/:

* **controllers.cpp** -> Contiene las funciones que son la implementación de los diferentes controladores
* **motorPWM.cpp** -> Contiene las funciones que manejan el PWM HW
* **main.cpp** -> Contiene el setup y loop principal además de las configuración de las interrupciones necesarias (encoder y timers)

Dentro de la carpeta python/:

* **originalData?/** -> Carpetas con los datos de los diferentes experimentos realizados
* **outputData?/** -> Carpetas con los datos de los diferentes experimentos realizados ya tratados (gráficos y datos)
* **fromDatatoFile.py** -> Script para tratar los datos obtenidos del experimento y generar gráficas (para obtener carpeta outputData a partir de originalData)
* **readSerial.py** -> Script que lee el puerto serie y almacena en el archivo que se indique
* **to_csv.py** -> Script que crea experimentos que serán leidos desde el Arduino