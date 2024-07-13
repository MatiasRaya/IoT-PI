from ftpadvanced import AdvancedFTP

# Se declara la clase Ftp
class Ftp:
    # Se crea el constructor de la clase
    def __init__(self, ftp_host, ftp_port, ftp_user, ftp_pass):
        # Se inicializa el objeto ftp de la clase AdvancedFTP
        self.ftp = AdvancedFTP(ftp_host, ftp_port)
        # Se realiza la conexión al servidor FTP
        self.ftp.login(ftp_user, ftp_pass)

    # Se crea el metodo list_files
    def list_files(self):
        # Se obtiene la lista de archivos del servidor FTP
        files = self.ftp.nlst()
        # Se retorna la lista de archivos
        return files
    
    # Se crea el metodo download_file
    def download_file(self, filename):
        # Se abre el archivo en modo escritura binaria
        with open(filename, 'wb') as file:
            # Se descarga el archivo del servidor FTP
            self.ftp.retrbinary('RETR ' + filename, file.write)
    
    # Se crea el metodo upload_file
    def upload_file(self, filename):
        # Se abre el archivo en modo lectura binaria
        with open(filename, 'rb') as file:
            # Se sube el archivo al servidor FTP
            self.ftp.storbinary('STOR ' + filename, file)