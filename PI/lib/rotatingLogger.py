import os
import machine
import time

class RotatingLogger:
    # Se crea el constructor de la clase
    def __init__(self, log_file, log_directory, maxFileSize, maxFiles):
        # Se almacena el nombre del archivo de log
        self.log_file = log_file
        # Se almacena el directorio de log
        self.log_directory = log_directory
        # Se almacena el tamaño maximo del archivo
        self.maxFileSize = maxFileSize
        # Se almacena la cantidad maxima de archivos
        self.maxFiles = maxFiles

        # Se crea un objeto de la clase machine para acceder a la tarjeta SD
        sd = machine.SD()

        # Se monta la tarjeta SD
        os.mount(sd, '/sd')

        # Se obitienen todos los directorios de la tarjeta SD
        dirs = os.listdir('/sd')

        # Se verifica si el directorio de log no existe en la tarjeta SD
        if not self.log_directory in dirs:
            # Se imprime un mensaje indicando que el directorio de log no existe en la tarjeta SD
            print('El directorio de log no existe en la tarjeta SD')
            # Se crea el directorio de log en la tarjeta SD
            os.mkdir('/sd/' + self.log_directory)
            # Se imprime un mensaje indicando que se creo el directorio de log
            print('Se creo el directorio de log en: {}'.format('/sd/' + self.log_directory))
        else:
            # Se imprime un mensaje indicando que el directorio de log ya existe en la tarjeta SD
            print('El directorio de log ya existe en: {}'.format('/sd/' + self.log_directory))

    # Se declara un metodo para rotar el archivo de log
    def rotate(self):
        # Se obtienen los elementos del directorio de log
        dirs = os.listdir('/sd/' + self.log_directory)

        # Se verifica si el archivo de log existe en el directorio de log
        if self.log_file in dirs:
            # Se imprime un mensaje indicando que el archivo de log existe en el directorio de log
            print('El archivo de log existe en el directorio de log')

            # Se obtiene el tamaño del archivo de log
            fileSize = os.stat('/sd/' + self.log_directory + '/' + self.log_file)[6]

            print('Tamaño del archivo de log: {}'.format(fileSize))
            print('Tamaño maximo del archivo: {}'.format(self.maxFileSize))

            # Se verifica si el tamaño del archivo de log es mayor al tamaño maximo del archivo
            if fileSize > self.maxFileSize:
                # Se imprime un mensaje indicando que el tamaño del archivo de log es mayor al tamaño maximo del archivo
                print('El tamaño del archivo de log es mayor al tamaño maximo del archivo')

                # Se obtiene la fecha y hora actual parseada
                date = time.localtime()
                # Se obtiene la fecha y hora actual formateada
                date = '{}-{}-{}_{}-{}-{}'.format(date[0], date[1], date[2], date[3], date[4], date[5])

                # Se renombra el archivo de log usando la fecha y hora actual
                os.rename('/sd/' + self.log_directory + '/' + self.log_file, '/sd/' + self.log_directory + '/' + self.log_file + '-' + date + '.log')
                # Se imprime un mensaje indicando que el archivo de log fue renombrado
                print('El archivo de log fue renombrado')

                # Se crea un nuevo archivo de log
                with open('/sd/' + self.log_directory + '/' + self.log_file, 'w') as file:
                    # Se imprime un mensaje indicando que se creo un nuevo archivo de log
                    print('Se creo un nuevo archivo de log')

                # Se imprime un mensaje indicando que se completo la rotacion del archivo de log
                print('Se completo la rotacion del archivo de log')

                # Se limita la cantidad de archivos de log
                self.limit_logs()
            else:
                # Se imprime un mensaje indicando que el tamaño del archivo de log no es mayor al tamaño maximo del archivo
                print('El tamaño del archivo de log no es mayor al tamaño maximo del archivo')
        else:
            # Se imprime un mensaje indicando que el archivo de log no existe en el directorio de log
            print('El archivo de log no existe en el directorio de log')

        # Se imprime el contenido del directorio de log
        print('Contenido del directorio de log: {}'.format(dirs))

    # Se declara un metodo para escribir un mensaje en el archivo de log
    def log(self, message):
        # Se obtienen los elementos del directorio de log
        dirs = os.listdir('/sd/' + self.log_directory)

        # Se verifica si el archivo de log existe en el directorio de log
        if self.log_file in dirs:
            # Se imprime un mensaje indicando que el archivo de log ya existe en el directorio de log
            print('El archivo de log ya existe en el directorio de log')
        else:
            # Se imprime un mensaje indicando que el archivo de log no existe en el directorio de log
            print('El archivo de log no existe en el directorio de log')

            # Se crea el archivo de log en el directorio de log
            with open('/sd/' + self.log_directory + '/' + self.log_file, 'w') as file:
                # Se imprime un mensaje indicando que se creo el archivo de log
                print('Se creo el archivo de log en el directorio de log')

        # Se guarda el mensaje en el archivo de log
        with open('/sd/' + self.log_directory + '/' + self.log_file, 'a') as file:
            # Se escribe el mensaje en el archivo de log
            file.write(message + '\n')

        # Se imprime un mensaje indicando que se escribio el mensaje en el archivo de log
        print('Se escribio el mensaje en el archivo de log')

        # Se imprime el contenido del archivo de log
        with open('/sd/' + self.log_directory + '/' + self.log_file, 'r') as file:
            # Se lee el contenido del archivo de log
            content = file.read()
            # Se imprime el contenido del archivo de log
            print('El contenido del archivo de log es:')
            print(content)

        # Se rota el archivo de log
        self.rotate()
    
    # Se declara un metodo para limitar la cantidad de archivos de log
    def limit_logs(self):
        # Se obtienen los elementos del directorio de log
        dirs = os.listdir('/sd/' + self.log_directory)

        # Se obtienen los archivos de log
        log_files = [f for f in dirs if f.startswith(self.log_file)]

        for log_file in log_files:
            file_stat = os.stat('/sd/' + self.log_directory + '/' + log_file)
            creation_time = time.localtime(file_stat[8])
            formatted_time = '{}-{}-{}_{}-{}-{}'.format(creation_time[0], creation_time[1], creation_time[2], creation_time[3], creation_time[4], creation_time[5])
            print('Archivo: {}, Fecha de creacion: {}'.format(log_file, formatted_time))

        # Se verifica si la cantidad de archivos de log es mayor a la cantidad maxima de archivos
        if len(log_files) > self.maxFiles:
            # Se ordenan los archivos de log por fecha de creacion, poniendo los mas antiguos al principio
            log_files.sort(key=lambda x: os.stat('/sd/' + self.log_directory + '/' + x)[8])

            print('Archivos de log: {}'.format(log_files))

            # Se eliminan los archivos de log mas antiguos
            while len(log_files) > self.maxFiles:
                # Se imprime un mensaje indicando que archivos de log se eliminaran
                print('Archivos de log que se eliminaran: {}'.format(log_files[0]))

                # Se elimina un archivo de log antiguo
                os.remove('/sd/' + self.log_directory + '/' + log_files.pop(0))
        else:
            # Se imprime un mensaje indicando que la cantidad de archivos de log no es mayor a la cantidad maxima de archivos
            print('La cantidad de archivos de log no es mayor a la cantidad maxima de archivos')