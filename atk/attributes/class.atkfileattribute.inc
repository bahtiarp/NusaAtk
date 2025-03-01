<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage attributes
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6309 $
   * $Id: class.atkfileattribute.inc 6494 2009-09-02 13:40:22Z jvansluijs $
   */

  useattrib("atkattribute");

  /** flag(s) specific for the atkFileAttribute */

  /**
   * Disable uploading of files
   */
  define("AF_FILE_NO_UPLOAD",           AF_SPECIFIC_1);

  /**
   * Disable selecting of files
   */
  define("AF_FILE_NO_SELECT",           AF_SPECIFIC_2);

  /**
   * Disable deleting of files
   */
  define("AF_FILE_NO_DELETE",           AF_SPECIFIC_3);

  /**
   * Don't try to detect the file type (shows only filename)
   */
  define("AF_FILE_NO_AUTOPREVIEW",      AF_SPECIFIC_4);

  /**
   * Removed the files physically
   */
  define("AF_FILE_PHYSICAL_DELETE",     AF_SPECIFIC_5);

  /**
   * Show preview in popup instead of inline
   */
  define("AF_FILE_POPUP",                    AF_POPUP);

  /**
   * With this you can upload, select and remove files in a given directory.
   *
   * @todo - Code clean up (del variable is dirty)
   *       - Support for storing the file itself in the db instead of on disk.
   *
   * @author Martin Roest <martin@ibuildings.nl>
   * @package atk
   * @subpackage attributes
   *
   */
  class atkFileAttribute extends atkAttribute
  {
    /**
     * Directory with images
     */
    var $m_dir = "";
    var $m_url = "";

    /**
     * Name mangle feature. If you set filename tpl, then uploaded files
     * are renamed to what you set in the template. You can use
     * fieldnames between brackets to have the filename determined by
     * the record.
     *
     * This is useful in the following case:
     * Say, you have a class for managing users. Each user has a photo associated
     * with them. Now, if two users would upload 'gandalf.gif', then you would
     * have a naming conflicht and the picture of one user is overwritten with the
     * one from the other user.
     * If you set m_filenameTpl to "picture_[name]", then the file is renamed before
     * it is stored. If the user's name is 'Ivo Jansch', and he uploads 'gandalf.gif',
     * then the file that is stored is picture_Ivo_Jansch.gif. This way, you have a
     * unique filename per user.
     */
    var $m_filenameTpl = "";

    /**
     * When set to true, a file is auto-renumbered if another record exists with the
     * same filename.
     *
     * @var boolean
     */
    var $m_autonumbering = false;

    /**
     * List of mime types which a uses is allowed to upload
     * Example: array('image/jpeg');
     *
     * @var array
     */
    var $m_allowedFileTypes = array();

    /**
     * Constructor
     * @param string $name Name of the attribute
     * @param array $dir Can be a string with the Directory with images/files or an array with a Directory and a Display Url
     * @param int $flags Flags for this attribute
     * @param int $size Filename size
     */
    function atkFileAttribute($name, $dir, $flags=0, $size=0)
    {
      // We use 255 as default filename size if no size specified.
      if ($size==0) $size = 255;

      // Call base class constructor.
      $this->atkAttribute($name,$flags|AF_CASCADE_DELETE, $size);
      $this->setDir($dir);
    }

    function __construct($name, $dir, $flags=0, $size=0)
    {
      // We use 255 as default filename size if no size specified.
      if ($size==0) $size = 255;

      // Call base class constructor.
      $this->atkAttribute($name,$flags|AF_CASCADE_DELETE, $size);
      $this->setDir($dir);
    }

    /**
     * Sets the directory into which uploaded files are saved.  (See setAutonumbering() and setFilenameTemplate()
     * for some other ways of manipulating the names of uploaded files.)
     *
     * @param mixed $dir string with directory path or array with directory path and display url (see constructor)
     */
    public function setDir($dir)
    {
      if(is_array($dir))
      {
        $this->m_dir = $this->AddSlash($dir[0]);
        $this->m_url = $this->AddSlash($dir[1]);
      }
      else
      {
        $this->m_dir = $this->AddSlash($dir);
        $this->m_url = $this->AddSlash($dir);
      }
    }

    /**
     * Turn auto-numbering of filenames on/off.
     *
     * When autonumbering is turned on, uploading a file with the same name as
     * the file of another record, will result in the file getting a unique
     * sequence number.
     *
     * @param bool $autonumbering
     */
    function setAutonumbering($autonumbering=true)
    {
      $this->m_autonumbering = $autonumbering;
    }


    /**
     * returns a string with a / on the end
     * @param string $dir_url String with the url/dir
     * @return string with a / on the end
     */
    function AddSlash($dir_url)
    {
      if(substr($dir_url,strlen($dir_url)-1,strlen($dir_url))!="/") { $dir_url.="/"; }
      return $dir_url;
    }


    /**
     * Returns an array containing files in specified directory
     * optionally filtered by settings from setAllowedFileTypes method.
     *
     * @param string $dir Directory to read files from
     * @return array Array with files in specified dir
     */
    function getFiles($dir)
    {
      $dirHandle = dir($this->m_dir);
      $file_arr  = array();

      while($item = $dirHandle->read())
      {
        if (count((array)$this->m_allowedFileTypes) == 0)
        {
          if(is_file($this->m_dir.$item)) $file_arr[] = $item;
        }
        else
        {
          $extension = $this->getFileExtension($item);

          if (in_array($extension, $this->m_allowedFileTypes))
          {
            if(is_file($this->m_dir.$item)) $file_arr[] = $item;
          }
        }
      }
      $dirHandle->close();
      return $file_arr;
    }


    /**
     * Returns a piece of html code that can be used in a form to edit this
     * attribute's value.
     * @param array $record Record
     * @param string $fieldprefix Field prefix
     * @param String $mode The mode we're in ('add' or 'edit')
     * @return string piece of html code with a browsebox
     */
    function edit($record="", $fieldprefix="", $mode="")
    {
      if (method_exists($this->getOwnerInstance(), $this->fieldName().'_display'))
      {
        $method = $this->fieldName().'_display';
        $result = $this->m_ownerInstance->$method($record, 'view');
      }
      else
      {
        $result=$this->display($record, 'view');
      }

      if(!is_dir($this->m_dir) || !is_writable($this->m_dir))
      {
        atkwarning('atkFileAttribute: ' . $this->m_dir . ' does not exist or is not writeable');
        return atktext("no_valid_directory", "atk");
      }

      $id = $fieldprefix.$this->fieldName();

      if ($result!="")
      {
        $result.="<br>";
        $result.='<input type="hidden" name="'.$id.'_orgfilename" value="'.$record[$this->fieldName()]['orgfilename'].'">';
      }

      $result.='<input type="hidden" name="'.$id.'_postfileskey" value="'.$id.'">';

      $onchange = '';
      if (count((array)$this->m_onchangecode))
      {
        $onchange = ' onchange="'.$id.'_onChange(this);"';
        $this->_renderChangeHandler($fieldprefix);
      }

      if(!$this->hasFlag(AF_FILE_NO_UPLOAD))
      {
        $this->registerKeyListener($id, KB_CTRLCURSOR|KB_UPDOWN);
        $result .= '<input type="file" id="'.$id.'" name="'.$id.'" '.$onchange.'>';
      }

      if(!$this->hasFlag(AF_FILE_NO_SELECT))
      {
        $file_arr = $this->getFiles($this->m_dir);
        if(count((array)$file_arr)>0)
        {
          natcasesort($file_arr);

          $this->registerKeyListener($id.'_select', KB_CTRLCURSOR|KB_LEFTRIGHT);
          $result .= '<select id="'.$id.'_select" name="'.$id.'[select]" '.$onchange.'>';
          // Add default option with value NULL
          $result .= "<option value=\"\" selected>".atktext('selection', 'atk');
          while (list ($key, $val) = each ($file_arr))
          {
            (isset($record[$this->fieldName()]['filename']) && $record[$this->fieldName()]['filename']==$val) ? $selected = "selected" : $selected = '';
            if(is_file($this->m_dir.$val)) $result .= "<option value=\"".$val."\" $selected>".$val;
          }
          $result .= "</select>";
        }
      }
      else if (isset($record[$this->fieldName()]['filename']) && !empty($record[$this->fieldName()]['filename']))
      {
        $result .= '<input type="hidden" name="'.$id.'[select]" value="'.$record[$this->fieldName()]['filename'].'">';
      }

      if(!$this->hasFlag(AF_FILE_NO_DELETE)
         && isset($record[$this->fieldname()]['orgfilename'])
         && $record[$this->fieldname()]['orgfilename']!='')
      {
       $this->registerKeyListener($id.'_del', KB_CTRLCURSOR|KB_CURSOR);
       $result .= '<br><input id="'.$id.'_del" type="checkbox" name="'.$id.'[del]" '.$this->getCSSClassAttribute("atkcheckbox").'>&nbsp;'.atktext("remove_current_file", "atk");
      }
      return $result;
    }

    /**
     * Convert value to record for database
     * @param array $rec Array with Fields
     * @return string Nothing or Fieldname or Original filename
     */
    function value2db($rec)
    {
      $del = (isset($rec[$this->fieldName()]['postdel']))?$rec[$this->fieldName()]['postdel']:null;

      if($rec[$this->fieldName()]["tmpfile"]=="" && $rec[$this->fieldName()]["filename"]!=""
         && (!isset($del) || $del!=$rec[$this->fieldName()]['filename']))
      {
        return $this->escapeSQL($rec[$this->fieldName()]["filename"]);
      }
      if($this->hasFlag(AF_FILE_NO_DELETE)) unset($del);  // Make sure if flag is set $del unset!

      if(isset($del))
      {
        if($this->hasFlag(AF_FILE_PHYSICAL_DELETE))
        {
          $file = "";
          if (isset($rec[$this->fieldName()]["postdel"]) && $rec[$this->fieldName()]["postdel"]!="")
          {
            atkdebug("postdel set");
            $file = $rec[$this->fieldName()]["postdel"];
          }
          else if ( isset($rec[$this->fieldName()]["orgfilename"]))
          {
            atkdebug("postdel not set");
            $file = $rec[$this->fieldName()]["orgfilename"];
          }
          atkdebug("file is now ".$file);
          if ($file!="" && file_exists($this->m_dir.$file))
          {
            unlink($this->m_dir.$file);
          }
          else
            atkdebug("File doesn't exist anymore.");
        }
//        echo ':::::return leeg::::';
        return '';
      }
      else
      {
        $filename = $rec[$this->fieldName()]["filename"];
        // why copy if the files are the same?

        if ($this->m_dir.$filename != $rec[$this->fieldName()]["tmpfile"])
        {
            if ($filename!="")
            {
              $dirname = dirname($this->m_dir.$filename);
              if (!$this->mkdir($dirname))
              {
                atkerror("File could not be saved");
                return "";
              }

              if (@move_uploaded_file($rec[$this->fieldName()]["tmpfile"],$this->m_dir.$filename))
              {
                $this->processFile($this->m_dir, $filename);
                return $this->escapeSQL($filename);
              }
              else
              {
                atkerror("File could not be saved");
                return "";
              }
            }

        }

        return $this->escapeSQL($rec[$this->fieldName()]["orgfilename"]);
      }
    }

    /**
     * Recursive mkdir.
     *
     * @see http://nl2.php.net/mkdir
     *
     * @param string $path path to create
     * @return success/failure
     *
     * @static
     */
    public static function mkdir($path)
    {
      $path = preg_replace('/(\/){2,}|(\\\){1,}/','/', $path); //only forward-slash
      $dirs = explode("/", $path);

      $path = "";
      foreach ($dirs as $element)
      {
        $path .= $element."/";
        if(!is_dir($path) && !mkdir($path))
          return false;
      }

      return true;
    }

    /**
     * Recursive rmdir.
     *
     * @see http://nl3.php.net/rmdir
     *
     * @param string $dir path to remove
     * @return succes/failure
     *
     * @static
     */
    function rmdir($dir)
    {
      if (!($handle = @opendir($dir))) return false;

      while (false !== ($item = readdir($handle)))
      {
        if ($item != "." && $item != "..")
        {
          if (is_dir("$dir/$item"))
          {
            if (!atkFileAttribute::rmdir("$dir/$item"))
              return false;
          }
          else
          {
            if (!@unlink("$dir/$item"))
              return false;
          }
        }
      }
      closedir($handle);
      return @rmdir($dir);
    }

    /**
     * Perform processing on an image right after it is uploaded.
     *
     * If you need any resizing or other postprocessing to be done on a file
     * after it is uploaded, you can create a derived attribute that
     * implements the processFile($filepath) method.
     * The default implementation does not do any processing.
     * @param String $filepath The path of the uploaded file.
     * @param String $filename The name of the uploaded file.
     */
    function processFile($filepath, $filename)
    {
      /* Added by Ellyas on March 19, 2013 */
   //   $jenis = $this->getFileExtension($filename);
  //    $this->thumb($filename, $filepath, $jenis);
    }

    /**
     * Set the allowed file types. This can either be mime types (detected by the / in the middle
     * or file extensions (without the leading dot!).
     *
     * @param array $types
     * @return boolean
     */
    function setAllowedFileTypes($types)
    {
      if(!is_array($types))
      {
        atkerror('atkFileAttribute::setAllowedFileTypes() Invalid types (types is not an array!');
        return false;
      }
      $this->m_allowedFileTypes = $types;
      return true;
    }

    /**
     * Check whether the filetype is is one of the allowed
     * file formats. If the FileType array is empty this assumes that
     * all formats are allowed!
     *
     * @todo It turns out that handling mimetypes is not that easy
     * the mime_content_type has been deprecated and there is no
     * Os independend alternative! For now we only support a few
     * image mime types.
     *
     * @param array $rec
     * @return boolean
     */
    function isAllowedFileType(&$rec)
    {
      if (count((array)$this->m_allowedFileTypes) == 0) return true;

      // detect whether the file is uploaded or is an existing file.
      $filename =  (!empty($rec[$this->fieldName()]['tmpfile'])) ?
        $rec[$this->fieldName()]['tmpfile'] :
        $this->m_dir . $rec[$this->fieldName()]['filename'];

      if (@empty($rec[$this->fieldName()]['postdel']) && $filename != $this->m_dir)
      {
        $valid = false;

        if (function_exists('getimagesize'))
        {
          $size = @getimagesize($filename);
          if (in_array($size['mime'], $this->m_allowedFileTypes))
          {
            $valid = true;
          }
        }

        $orgFilename = @$rec[$this->fieldName()]['orgfilename'];
        if ($orgFilename != null)
        {
          $extension = $this->getFileExtension($orgFilename);
          if (in_array($extension, $this->m_allowedFileTypes))
          {
            $valid = true;
          }
        }

        if (!$valid)
        {
          $rec[$this->fieldName()]['error'] = UPLOAD_ERR_EXTENSION;
          return false;
        }
      }

      return true;
    }

    /**
     * Convert value to string
     * @param array $rec Array with fields
     * @return array Array with tmpfile, orgfilename,filesize
     */
    function db2value($rec)
    {
      return Array("tmpfile"=>$this->m_dir.$rec[$this->fieldName()],
                   "orgfilename"=>$rec[$this->fieldName()],
                   "filename"=>$rec[$this->fieldName()],"filesize"=>"?");
    }

    /**
     * Checks if the file has a valid filetype.
     *
     * Note that obligatory and unique fields are checked by the
     * atkNodeValidator, and not by the validate() method itself.
     *
     * @param array $record The record that holds the value for this
     *                      attribute. If an error occurs, the error will
     *                      be stored in the 'atkerror' field of the record.
     * @param String $mode The mode for which should be validated ("add" or
     *                     "update")
     */
    function validate(&$record, $mode)
    {
      parent::validate($record, $mode);

      $this->isAllowedFileType($record);

      $error = $record[$this->fieldName()]['error'];
      if ($error > 0)
      {
        $error_text = $this->fetchFileErrorType($error);
        atkTriggerError($record, $this, $error_text, atktext($error_text, "atk"));
      }
    }

    /**
     * Tests the $_FILE error code and returns the corresponding atk error text token.
     *
     * @param int $error
     * @return string error text token
     */
    static function fetchFileErrorType($error) {
        switch ($error)
        {
          case UPLOAD_ERR_INI_SIZE:
          case UPLOAD_ERR_FORM_SIZE:
            $error = 'error_file_size';
            break;
          case UPLOAD_ERR_EXTENSION:
            $error = 'error_file_mime_type';
            break;
          case UPLOAD_ERR_NO_TMP_DIR:
          case UPLOAD_ERR_PARTIAL:
          default:
            $error = 'error_file_unknown';
        }

        return $error;
    }

    /**
     * Get filename out of Array
     * @param array $rec Record
     * @return array Array with tmpfile,filename,filesize,orgfilename
     */
    function fetchValue($rec)
    {
      $del = (isset($rec[$this->fieldName()]['del']))?$rec[$this->fieldName()]['del']:null;

      $postfiles_basename = $rec[$this->fieldName()."_postfileskey"];

      $basename = $this->fieldName();

      if (is_array($_FILES)||($rec[$this->fieldName()]["select"] != "") || ($rec[$this->fieldName()]["filename"]!="")) // php4
      {
        // if an error occured during the upload process
        // and the error is not 'no file' while the field isn't obligatory or a file was already selected
        $fileselected = isset($rec[$this->fieldName()]["select"]) && $rec[$this->fieldName()]["select"] != "";
        if ($_FILES[$postfiles_basename]['error'] > 0 && !((!$this->hasFlag(AF_OBLIGATORY) || $fileselected) && $_FILES[$postfiles_basename]['error'] == UPLOAD_ERR_NO_FILE))
        {
          return array('error' => $_FILES[$postfiles_basename]['error']);
        }

        // if no new file has been uploaded..
        elseif (count((array)$_FILES)==0||$_FILES[$postfiles_basename]["tmp_name"]=="none"||$_FILES[$postfiles_basename]["tmp_name"]=="")
        {
           // No file to upload, then check if the select box is filled
          if($fileselected)
          {
            atkdebug("file selected!");
            $filename=$rec[$this->fieldName()]["select"];
            $orgfilename = $filename;
            $postdel = '';
            if (isset($del) && $del=="on")
            {
              $filename = '';
              $orgfilename='';
              $postdel = $rec[$this->fieldName()]["select"];
            }
            $result = array("tmpfile"=>"",
                          "filename"=>$filename,
                          "filesize"=>0,
                          "orgfilename"=>$orgfilename,
                          "postdel"=>$postdel);
          }  // maybe we atk restored data from session
          elseif(isset($rec[$this->fieldName()]["filename"]) && $rec[$this->fieldName()]["filename"]!="")
          {
            $result = $rec[$this->fieldName()];
          }
          else
          {
            $filename = (isset($rec[$basename."_orgfilename"]))?$rec[$basename."_orgfilename"]:"";

            if (isset($del) && $del=="on")
            {
              $filename = '';
            }

            // Note: without file_exists() check, calling filesize() generates an error message:
            $result = array("tmpfile"=>$filename == '' ? '' : $this->m_dir.$filename,
                          "filename"=>$filename,
                          "filesize"=>($filename!='' && file_exists(filesize($this->m_dir.$filename)) ? filesize($this->m_dir.$filename) : 0),
                          "orgfilename"=>$filename);
          }
        }
        else
        {
          $realname = $this->_filenameMangle($rec, $_FILES[$postfiles_basename]["name"]);

          if ($this->m_autonumbering)
          {
            $realname = $this->_filenameUnique($rec, $realname);
          }

          $result = array("tmpfile"=>$_FILES[$postfiles_basename]["tmp_name"],
                          "filename"=>$realname,
                          "filesize"=>$_FILES[$postfiles_basename]["size"],
                          "orgfilename"=>$realname);
        }
        return $result;
      }
    }

    /**
     * Check if the attribute is empty..
     *
     * @param array $record the record
     *
     * @return boolean true if empty
     */
    function isEmpty($record)
    {
      return @empty($record[$this->fieldName()]['filename']);
    }

    /**
     * Deletes file from HD
     * @param array $record Array with fields
     * @return boolean False if the delete went wrong
     */
    function delete($record)
    {
      if($this->hasFlag(AF_FILE_PHYSICAL_DELETE)&&($record[$this->fieldname()]["orgfilename"]!=""))
      {
        if(file_exists($this->m_dir.$record[$this->fieldName()]["orgfilename"]) && !@unlink($this->m_dir.$record[$this->fieldName()]["orgfilename"]))
        {
          return false;
        }
      }
      return true;
    }

    /**
     * Display values
     * @param array $record Array with fields
     * @return string Filename or Nothing
     */
    function display($record, $mode = '')
    {
      // Get random number to use as param when displaying images
      // while updating images was not allways visible due to caching
      $randval = mt_rand();

      $filename    = $record[$this->fieldName()]["filename"];
      atkdebug($this->fieldname()." - File: $filename");
      $prev_type     = Array("jpg","jpeg","gif","tif","png","bmp","htm","html","txt");  // file types for preview
      $imgtype_prev = Array("jpg","jpeg","gif","png");  // types whitch are supported by GetImageSize
      if($filename != "")
      {
        if(is_file($this->m_dir.$filename))
        {
          $ext = $this->getFileExtension($filename);
          if((in_array($ext, $prev_type) && $this->hasFlag(AF_FILE_NO_AUTOPREVIEW))||(!in_array($ext, $prev_type)))
          {
            return "<a href=\"".$this->m_url."$filename\" target=\"_blank\">$filename</a>";
          }
          elseif(in_array($ext, $prev_type) && $this->hasFlag(AF_FILE_POPUP))
          {
            if(in_array($ext, $imgtype_prev))
            {
              $imagehw = GetImageSize($this->m_dir.$filename);
            }
            else
            {
              $imagehw=Array("0"=>"640","1"=>"480");
            }
            $page = atkPage::getInstance();
            $page->register_script(atkconfig("atkroot")."atk/javascript/newwindow.js");
            return '<a href="'.$this->m_url.$filename.'" alt="'.$filename.'" onclick="NewWindow(this.href,\'name\',\''.($imagehw[0]+50).'\',\''.($imagehw[1]+50).'\',\'yes\');return false;">'.$filename.'</a>';
          }
          /* Edited by Ellyas on March 19, 2013 */
          // return '<img src="'.$this->m_url.$filename.'?b='.$randval.'" alt="'.$filename.'">';
          return '<div id="container_frame_foto" style="text-align: center;"><img id="master_frame_foto" style="-moz-border-bottom-colors: none; -moz-border-left-colors: none; -moz-border-right-colors: none; -moz-border-top-colors: none; -webkit-border-bottom-colors: none; -webkit-border-left-colors: none; -webkit-border-right-colors: none;  -webkit-border-top-colors: none; border-color: #FFFFFF; border-image: none; border-style: solid; border-width: 3px 3px 10px; box-shadow: 0 0 3px #8f8f8f;" src="'.$this->m_url. 'thumb_' . $filename.'?b='.$randval.'" alt="'.$filename.'"></div>';
        }
        else
        {
          return $filename."(<font color=\"#ff0000\">".atktext("file_not_exist", "atk")."</font>)";
        }
      }
    }

    /**
     * Get the file extension
     *
     * @param string $filename Filename
     * @return string The file extension
     */
    function getFileExtension($filename)
    {
      if($dotPos = strrpos($filename,'.'))
      {
        return strtolower(substr($filename, $dotPos+1,strlen($filename)));
      }
      return '';
    }

    /**
     * Retrieve the list of searchmodes which are supported.
     *
     * @return array List of supported searchmodes
     */
    function getSearchModes()
    {
      // exact match and substring search should be supported by any database.
      // (the LIKE function is ANSI standard SQL, and both substring and wildcard
      // searches can be implemented using LIKE)
      // Possible values
      //"regexp","exact","substring", "wildcard","greaterthan","greaterthanequal","lessthan","lessthanequal"
      return array("substring","exact","wildcard","regexp");
    }

    /**
     * Set filename template.
     *
     * @param string $template
     */
    function setFilenameTemplate($template)
    {
      $this->m_filenameTpl = $template;
    }

    /**
     * Determine the real filename of a file.
     *
     * If a method <fieldname>_filename exists in the owner instance this method
     * is called with the record and default filename to determine the filename. Else
     * if a file template is set this is used instead and otherwise the default
     * filename is returned.
     *
     * @access private
     * @param array $rec The record
     * @param string $default The default filename
     * @return The real filename
     */
    function _filenameMangle($rec, $default)
    {
      $method = $this->fieldName().'_filename';
      if (method_exists($this->m_ownerInstance, $method))
        return $this->m_ownerInstance->$method($rec, $default);
      else return $this->filenameMangle($rec, $default);
    }

    /**
     * Determine the real filename of a file (based on m_filenameTpl).
     * @access public
     * @param array $rec The record
     * @param string $default The default filename
     * @return The real filename based on the filename template
     */
    function filenameMangle($rec, $default)
    {
      if ($this->m_filenameTpl == "")
      {
        $filename = $default;
      }
      else
      {
        atkimport("atk.utils.atkstringparser");
        $parser = new atkStringParser($this->m_filenameTpl);
        $includes = $parser->getAttributes();
        $record = $this->m_ownerInstance->updateRecord($rec, $includes,array($this->fieldname()));
        $record[$this->fieldName()] = substr($default, 0, strrpos($default,'.'));
        $ext = $this->getFileExtension($default);
        $filename = $parser->parse($record).($ext!=''?".".$ext:'');
      }
      return str_replace(' ','_',$filename);
    }

    /**
     * Give the file a uniquely numbered filename.
     *
     * @access private
     * @param array $rec The record for thish the file was uploaded
     * @param String $filename The name of the uploaded file
     * @return String The name of the uploaded file, renumbered if necessary
     */
    function _filenameUnique($rec, $filename)
    {
      // check if there's another record using this same name. If so, (re)number the filename.
      atkdebug("atkFileAttribute::_filenameUnique() -> unique check");

      if ($dotPos = strrpos($filename,'.'))
      {
        $name = substr($filename, 0, strrpos($filename,'.'));
        $ext = substr($filename, strrpos($filename,'.'));
      }
      else
      {
        $name = $filename;
        $ext = "";
      }

      $selector = "(".$this->fieldName()."='$filename' OR ".$this->fieldName()." LIKE '$name-%$ext')";
      if ($rec[$this->m_ownerInstance->primaryKeyField()]!="")
      {
        $selector.= " AND NOT (".$this->m_ownerInstance->primaryKey($rec).")";
      }

      $records = $this->m_ownerInstance->selectDb("($selector)", "", array($this->fieldName()));

      if (count((array)$records)>0)
      {
        // Check for the highest number
        $max_count=0;
        foreach($records as $record)
        {
          $dotPos = strrpos($record[$this->fieldName()]["filename"],'.');
          $dashPos = strrpos($record[$this->fieldName()]["filename"],'-');
          if($dotPos!==false && $dashPos!==false)
          {
            $number = substr($record[$this->fieldName()]["filename"],($dashPos+1),($dotPos-$dashPos)-1);
          }
          elseif($dotPos===false && $ext==""  && $dashPos!==false)
          {
            $number = substr($record[$this->fieldName()]["filename"],($dashPos+1));
          }
          else continue;

          if(intval($number)>$max_count) $max_count=$number;

        }
        // file name exists, so mangle it with a number.
        $filename = $name."-".($max_count+1).$ext;
      }
      atkdebug("atkFileAttribute::_filenameUnique() -> New filename = ".$filename);
      return $filename;
    }

    /**
     * Returns a piece of html code that can be used in a form to display
     * hidden values for this attribute.
     * @param array $record Array with values
     * @param String $fieldprefix The fieldprefix to put in front of the name
     *                            of any html form element for this attribute.
     * @return string Piece of htmlcode
     */
    function hide($record = '', $fieldprefix = '')
    {
      $field = $record[$this->fieldName()];

      if (is_array($field))
        foreach ($field as $key => $value)
          $result .= '<input type="hidden" name="'.$fieldprefix.$this->formName().'['.$key.']" '.'value="'.$value.'">';

      else
        $result = '<input type="hidden" name="'.$fieldprefix.$this->formName().'" value="'.$field.'">';

      return $result;
    }

    /**
     * Return the database field type of the attribute.
     * @return "string" which is the 'generic' type of the database field for
     *         this attribute.
     */
    function dbFieldType()
    {
      return "string";
    }


  }
?>
