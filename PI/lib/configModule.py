# Se declara la clase ConfigModule
class ConfigModule:
    # Se crea el constructor de la clase
    def __init__(self, config_file):
        # Se almacena el archivo de configuracion
        self.config_file = config_file    
    
    # Se declara el método get_config
    def get_config(self):
        # Se crea un diccionario para almacenar la configuracion
        config = {}

        # Se imprime un mensaje indicando que se esta obteniendo la configuracion desde el archivo
        print('Obteniendo configuracion desde el archivo')
        
        try:
            # Se abre el archivo en modo lectura
            with open(self.config_file, 'r') as file:
                # Se lee el archivo linea por linea
                for line in file:
                    # Se elimina los espacios en blanco al inicio y al final de la linea
                    line = line.strip()
    
                    # Se verifica si la linea esta vacia o si comienza con un punto y coma o numeral
                    if not line or line.startswith(';') or line.startswith('#'):
                        # Se omite la linea
                        continue
    
                    # Se verifica si la linea comienza con un corchete y termina con un corchete
                    if line.startswith('[') and line.endswith(']'):
                        # Se obtiene el nombre de la seccion
                        current_section = line[1:-1].strip()
                        # Se almacena la seccion en el diccionario de configuracion
                        config[current_section] = {}
                    else:
                        # Se obtiene la clave y el valor de la linea
                        key, value = line.split('=', 1)
                        # Se eliminan los espacios en blanco al inicio y al final de la clave y el valor
                        key = key.strip()
                        value = value.strip()
    
                        # Se verifica si existe una seccion actual
                        if current_section:
                            # Se almacena la clave y el valor en la seccion actual
                            config[current_section][key] = value
                        else:
                            # Se almacena la clave y el valor en el diccionario de configuracion
                            config[key] = value
        except Exception as e:
            # Se imprime un mensaje indicando que ocurrio un error leyendo el archivo
            print("Error leyendo el archivo: {}".format(e))
        
        # Se retorna la configuracion
        return config