

//archivo de texto y retorna fastq
process pA{
	
  //directorio para guardar los resultados de un proceso
  publishDir "${params.outdir}/pA/", mode: "copy"

	input:
	 tuple val(prefix), path(fileIn)
	
	output:
	 tuple val(prefix), path("${prefix}.A.txt")

	script:
  if(params.debug == true){
  """
  # simulamos como que corremos el proceso
  echo process_A ${fileIn} ${prefix} 
  touch ${prefix}.A.txt
  """
  }else{
  """
  process_A ${fileIn} ${prefix}
  """
  }
}


//archivo de texto y retorna fastq
process pB{
  
  //directorio para guardar los resultados de un proceso
  publishDir "${params.outdir}/pB/", mode: "copy"

  input:
  tuple val(prefix), path(fileIn)
  
  output:
   tuple val(prefix), path("${prefix}.B.txt")

  script:
  if(params.debug == true){
  """
  # simulamos como que corremos el proceso
  echo process_B ${fileIn} ${prefix} 
  touch ${prefix}.B.txt
  """
  }else{
  """
  process_B ${fileIn} ${prefix}
  """
  }
}



//archivo de texto y retorna fastq
process pC{
  
  //directorio para guardar los resultados de un proceso
  publishDir "${params.outdir}/pC/", mode: "copy"

  input:
  tuple val(prefix), path(fileA), path(fileB)

  output:
   tuple val(prefix), path("${prefix}.C.txt")
   path("${prefix}.C.txt"), emit: fo

  script:
  if(params.debug == true){
  """
  # simulamos como que corremos el proceso
  echo process_C ${fileA} ${fileB} ${prefix}
  touch ${prefix}.C.txt
  """
  }else{
  """
  process_C ${fileA} ${fileB} ${prefix}
  """
  }
}

//implememntar proceso D que recibe todos lo inpunt de C

process pD{
  
  //directorio para guardar los resultados de un proceso
  publishDir "${params.outdir}/pD/", mode: "copy"

  input:
      path (all_files)
  output:
   path("result.D.txt")
  
  script:
  if(params.debug == true){
  """
  # simulamos como que corremos el proceso
  echo process_D ${all_files} 
  echo ${all_files}
  touch result.D.txt
  """
  }else{
  """
  process_D ${all_files}
  """
  }
}


//definir el orden de los processos 
//mediante la creacion de canales de comunicacion
workflow{

//leer la tupla y path desde un archivo csv
txtfiles=Channel.fromPath(params.csv_in) \
        | splitCsv(header:true) \
        | map {row -> tuple (row.id, file(row.pfile)) }
 //txtfiles.view()

 pA_res=pA(txtfiles)
 pB_res=pB(txtfiles)

 // [id, fileA1.....fileA10]
 //pA_res.view()
 // [id, fileA1.....fileA10]
 //pB_res.view()
 //implementamos un join de los canales A y B
 pC_in=pA_res.join(pB_res)
 
 pC_res=pC(pC_in)
 //obtenemos solamente los archivos de resultados del proceso C y ejecutamos un collect que guardamos en 
 //all_archivos_c

 all_archivos_c=pC.out.fo.collect()
 //mostramos el contenido del cannal all archivos
 // all_archivos_c.view()
 pD_res=pD(all_archivos_c)

 //finaliza el pipeline con 4 procesos

}