<?php
/*
updated:
10/22/06
- added functions isImage() and isPermitted() to show non-image files to show just file names

Original by Ivo
Modified by Jorge Garifuna <info@GariDigital.com>

This attribute displays images on thumbnail format in the list view, while treating other files as normal files
usage example:

$f = &$this->add(new photoAttribute("photo1", moduleDir("photo_album")."photos_files",  AF_FILE_NO_SELECT| AF_FILE_PHYSICAL_DELETE, 0, "./photos_files/",moduleDir("photo_album")."image_processor.php"),NULL,300);
$f->setAutonumbering(true);


*/
class photoAttribute extends atkFileAttribute
 {
     var $image_folder_path;
     var $module_name;
     var $processor_path;
     var $file_path;

    /**
     * Constructor
     * @param string $name Name of the attribute
     * @param array $dir Can be a string with the Directory with images/files or an array with a Directory and a Display Url
     * @param int $flags Flags for this attribute
     * @param int $size Filename size
     * @param varchar $image_folder_path The path folder of the image 
     * @param varchar $processor_path    The path URL to the file that displays the large image when thumbnail is clicked on
     */
     function photoAttribute($name, $dir, $flags=0, $size=0, $image_folder_path, $processor_path){
          $this->image_folder_path = $image_folder_path;
          $this->processor_path = $processor_path;
          $this->file_path = $dir;
          $this->atkFileAttribute($name, $dir, $flags, $size);
     }

   // By overriding this method, we can do processing on the uploaded image.
   // The path and filename of the image are passed as parameters.
 function processFile($path, $filename) 
   { 
  
     } 

 
 function display($record, $mode="")
   {
     // In recodlists, show the thumbnail instead of the larger image.
     if ($mode=="list")
     {
          $image = $this->image_folder_path.$record[$this->fieldName()]["orgfilename"];
          // this line is added to link the thumbnail so the larger image can be viewed when clicked on
          // $this->processor_path must exists of course so that this can work
          $filename = $record[$this->fieldName()]["orgfilename"];
          if(!empty($filename)){
          	
               if($this->isImage($filename)){ // image
                    $url = "<a  href='".$this->processor_path."?ACTION=GET_LARGE_IMAGE&CONTENT=".$image."'>";
                    return $url."<img alt='".$filename."' border='0' src='".$this->processor_path."?ACTION=GET_IMAGE&TYPE=FILE&CONTENT=".$image."'></a>";          
               }else{ // not image
                    if($this->isPermitted($filename)){ // can be downloaded with mime type
                         $url = "<a  href='".$this->processor_path."?ACTION=GET_LARGE_IMAGE&CONTENT=".$image."'>";
                         return $url.$filename."</a>";
                    }else{// cannot be download with mime type
                         $url = "<a  target=_blank href='".$this->file_path."/".$filename."'>";
                         return $url.$filename."</a>";                    
                    }
               }
          }          
     }
     return parent::display($record, $mode);
   }// end function

     function isImage($filename){
	   $file_extension = strtolower(substr(strrchr($filename,"."),1));
//print "$filename = $file_extension";
//die();
	   //This will set the Content-Type to the appropriate setting for the file
	   switch( $file_extension ) {
	     case "gif": 
	     case "png": 
	     case "jpeg": 
	     case "jpg": $returnValue=true; break;

	     default: $returnValue=false;
	   }
        return $returnValue;
     }// end funtion

     function isPermitted($filename){
	   $file_extension = strtolower(substr(strrchr($filename,"."),1));
//print "$filename = $file_extension";
//die();
	   //This will set the Content-Type to the appropriate setting for the file
	   switch( $file_extension ) {
	     case "php":
	     case "htm":
	     case "html":
	     case "txt": $returnValue=false; break;


	     default: $returnValue=true;
	   }
        return $returnValue;
     }// end funtion
          
 }// end class

?>
