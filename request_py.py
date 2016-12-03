# Codigo criado com base no JavaScript do codigo fonte do Portal da Transparencia 

import requests
from tqdm import tqdm

year = 2012
auxYear = 2012
month = 01
auxMonth = '01'
while year <= 2016:
    while month <= 12:
        url = "http://arquivos.portaldatransparencia.gov.br/downloads.asp?a="+str(auxYear)+"&m="+str(auxMonth)+"&consulta=Diarias"
        # r = requests.get(url)
        response = requests.get(url, stream=True)

        fileName = auxMonth + str(auxYear)
        with open(fileName, "wb") as handle:
            for data in tqdm(response.iter_content()):
                handle.write(data)
        # print len(r.content)
        month = month + 1
        auxMonth = month
        if month < 10:
            auxMonth = '0' + str(auxMonth)
        else:
            auxMonth = str(auxMonth)
        
    year = year + 1
    month = 1
    auxMonth = '01'
    auxYear = year