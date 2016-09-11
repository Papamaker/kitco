<?php
// Rapporte les erreurs d'exécution de script
error_reporting(E_ERROR | E_WARNING | E_PARSE);

require_once "BMP_Loader.php" ;

$bl=new BMP_Loader("souris.bmp");

$image = $bl->Get_Image();

print ("  { { B");

for ($i=0;$i<10;$i++) {
  if ($i!=0) {
    print("    { B");
  }
  
  for ($j=0;$j<16;$j++) {
    if (($j==8)) {
      print(",B");
    }
    if (imageColorat($image,$j,$i)==0) {
      print("1"); 
    } else {
      print (0);
    }
  }
  if ($i!=9) {
    print("},\n");
  } else {
    print("} },");
  }
}
print ("\n");
?>