<?php 

  /** 
   * @internal include baseclass. 
   */ 
//  include_once($config_atkroot."atk/attributes/class.atktextattribute.inc"); 

  define('WW_BASIC', 1); 
  define('WW_ACCESSIBILITY', 2); 
  define('WW_ALL', 4); 

  class MceAttribute extends atkAttribute 
  { 
    var $m_size_array = ""; 
    var $m_opt = ""; 
    var $m_bars = array( 
      1 => array( 
        'theme_advanced_buttons1' => "bold,italic,underline,strikethrough,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,formatselect,fontselect,fontsizeselect,separator,link,unlink,separator,forecolor,backcolor,separator,cut,copy,paste,separator,image", 
        'theme_advanced_buttons2' => "", 
        'theme_advanced_buttons3' => "" 
      ), 
      2 => array( 
        'theme_advanced_buttons1' => "cut,copy,paste,separator,removeformat,separator,link,unlink,anchor,separator,image,hr,charmap,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,tablecontrols,separator,visualaid", 
        'theme_advanced_buttons2' => "formatselect,fontselect,fontsizeselect,bold,italic,underline,separator,bullist,numlist,separator,outdent,indent,separator,undo,redo,separator,forecolor,backcolor", 
        'theme_advanced_buttons3' => "", 
        'theme_advanced_path_location' => "bottom", 
      ), 
      4 => array( 
        'theme_advanced_buttons1' => "bold,italic,underline,strikethrough,separator,forecolor,backcolor,separator,justifyleft,justifycenter,justifyright,justifyfull,separator,styleselect,formatselect", 
        'theme_advanced_buttons2' => "bullist,numlist,separator,outdent,indent,separator,undo,redo,separator,link,unlink,anchor,flash,image,cleanup,help,code", 
        'theme_advanced_buttons3' => "tablecontrols,separator,hr,removeformat,visualaid,separator,sub,sup,separator,charmap", 
        'theme_advanced_path_location' => "bottom", 
      ), 
    ); 
    var $m_templates = ""; 
    var $m_vflags = ""; 

    /** 
     * Constructor 
     * @param String $name Name of the attribute 
     * @param int $flags Flags for the attribute 
     * @param int $visual_flags Flags for displaying the attribute 
     * @param array $opt Extra mce configuration options. 
     */ 
    function MceAttribute($name, $size_array=array(700, 300), $flags=0, $vflags=WW_BASIC, $templates = null, $opt=array()) 
    { 
      $this->atkAttribute($name,$flags); // base class constructor 
      
      $this->m_size_array = $size_array; 

      $this->m_vflags = $vflags; 

      $this->m_templates = $templates; 

      $this->m_opt = $opt; 

    } 


    /** 
     * Returns a piece of html code that can be used in a form to edit this 
     * attribute's value. 
     * @param $record Array with fields 
     * @return piece of html code with a textarea 
     */ 
    function edit($record="", $fieldprefix="") 
    { 
      global $config_atkroot; 

      $id = $fieldprefix.$this->fieldName(); 
      $this->registerKeyListener($id, KB_CTRLCURSOR); 

      $result= '<textarea id="'.$id.'" name="'.$id.'" style="width: '.$this->m_size_array[0].'px; height: '.$this->m_size_array[1].'px;" '. 
               '>'.htmlspecialchars($record[$this->fieldName()]).'</textarea>'; 

      $this->registerMce(); 

      if(is_array($this->m_templates)) 
      { 
        $page = &atkPage::getInstance(); 

        $js .= "\n\nvar templates_{$this->m_name} = new Array("; 
        $ui = &$this->m_ownerInstance->getUi(); 
        foreach($this->m_templates as $template_key => $template_value) 
        { 
          $html_template = ereg_replace("[\r\t]", '', $ui->render($template_value)); 
          $html_template = ereg_replace("[\n]", '\n', $html_template); 
          $html_template = str_replace("'", "\'", $html_template); 
          $html_template = str_replace("  ", " ", $html_template); 
          $js .= "'$html_template',\n"; 
        } 
        $js .= "'a');"; 
        $js .= "\n\nfunction AppendContent_{$this->m_name}(){\n"; 
        $js .= "  var index = document.forms[0].template_choice_{$this->m_name}.selectedIndex;\n"; 
        $js .= "  tinyMCE.execInstanceCommand('{$this->m_name}', 'mceInsertContent',false, templates_{$this->m_name}[index]);\n"; 
        $js .= "}\n"; 
        $page->register_scriptcode($js); 

        $result .= "<br><select name=\"template_choice_{$this->m_name}\">"; 
        foreach($this->m_templates as $template_key => $template_value) 
        { 
          $result .= '<option value="'.$template_key.'">'.$template_key.'</option>'; 
        } 
        $result .= '</select>'; 
        $result .= "<input type=\"button\" name=\"template_append\" value=\"".text('mce_template_apply')."\" onClick=\"AppendContent_{$this->m_name}()\">"; 
      } 

      return $result; 
    } 

    function registerMce(){ 
      $page = &atkPage::getInstance(); 

      //$file = "/tools/tinymce/jscripts/tiny_mce/tiny_mce.js"; 
      $file = moduleDir("gktools")."tinymce/jscripts/tiny_mce/tiny_mce.js"; 
      //moduleDir("gktools")."js/gktools_webstore.js" 

      $page->register_script($file); 

      //$file = "/tools/filemanager/filemanager.js"; 

      //$page->register_script($file); 

      $mce_opt=array( 
        'mode' => "exact", 
        'theme' => "advanced", 
        'elements' => $this->m_name, 
        'trim_span_elements' => false, 
        'verify_css_classes' => false, 
        'verify_html' => false, 
        'inline_styles' => false, 
        'remove_linebreaks' => false, 
        'plugins' => "table,searchreplace,contextmenu,advimage,advlink,flash", 
        'theme_advanced_toolbar_location' => "top", 
        'theme_advanced_toolbar_align' => "left", 
        'content_css' => "/tools/config.style.css", 
        'plugin_insertdate_dateFormat' => "%d-%m-%Y", 
        'plugin_insertdate_timeFormat' => "%H:%M:%S", 
        'extended_valid_elements' => "a[name|href|target|title|onclick],img[class|src|border=0|alt|title|hspace|vspace|width|height|align|onmouseover|onmouseout|name],hr[class|width|size|noshade],font[face|size|color|style],span[class|align|style]", 
        "file_browser_callback" => "FileManager.filebrowserCallBack" 
        ); 

      $style = $this->m_bars[$this->m_vflags]; 
      if(is_array($style)) 
        $mce_opt=array_merge($mce_opt, $style); 

      $config_mce = atkconfig('mce'); 
      if(is_array($config_mce)) 
        $mce_opt=array_merge($mce_opt,$config_mce); 

      if(is_array($this->m_opt)) 
        $mce_opt=array_merge($mce_opt,$this->m_opt); 

      $mce_opt['language'] = atkconfig("language"); 

      $js = "redesign = true;\n"; 
      $js .= "tinyMCE.init({\n"; 
      $atts = array(); 
      foreach($mce_opt as $key => $val){ 
        $atts[] = $key.' : "'.$val.'"'; 
      } 
      $js .= implode(", \n", $atts); 
      $js .= "})\n"; 

      $js .= "var lang_edit = '';\n"; 

      $page->register_scriptcode($js); 
    } 

    function display($record) 
    { 
      return "<div style='font-size: 11px;'>" . $record[$this->fieldName()] . '</div>'; 
    } 

  } 

?>
