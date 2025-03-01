<?php
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage ui
   *
   * @copyright (c)2000-2006 Ivo Jansch
   * @copyright (c)2000-2006 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6309 $
   * $Id: class.atktheme.inc 6561 2009-11-11 12:41:58Z peter $
   */

  /**
   * Theme loader
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @author Boy Baukema <boy@ibuildings.nl>
   * @package atk
   * @subpackage ui
   *
   */
  class atkTheme
  {
    var $m_name ="";
    var $m_theme = array();

    /**
     * Function to get an Instance of the atkTheme class,
     * ensures that there is never more than one instance (Singleton pattern)
     * 
     * @param bool $reset Always reset and return a new instance
     * @return atkTheme theme instance
     */
    public static function getInstance($reset=false)
    {
      static $s_instance = NULL;
      if ($s_instance == NULL || $reset)
      {
        $s_instance = new atkTheme();
      }
      return $s_instance;
    }

    /**
     * Constructor, initializes class and certain values
     * @access private
     */
    function __construct()
    {
      global $g_theme;
      atkdebug("Created a new atkTheme instance");
      if (isset($g_theme["Name"]) && $g_theme["Name"]!="")
      {
        $this->m_name = $g_theme["Name"];
      }
      else
      {
        $this->m_name = atkconfig("defaulttheme");
      }
      $this->_loadTheme();
    }

    function atkTheme()
    {
      global $g_theme;
      atkdebug("Created a new atkTheme instance");
      if (isset($g_theme["Name"]) && $g_theme["Name"]!="")
      {
        $this->m_name = $g_theme["Name"];
      }
      else
      {
        $this->m_name = atkconfig("defaulttheme");
      }
      $this->_loadTheme();
    }

    /**
     * Convert a relative theme path to an absolute path.
     *
     * If a relative path starts with 'module/something' this method converts
     * the start of the path to the location where the module 'something' is
     * actually installed.
     *
     * @static
     * @param String $relpath The relative path to convert
     * @param String $location 
     * @return String The absolute path
     */
    public static function absPath($relpath, $location='')
    {
      if ($relpath=="") return "";

      if (preg_match("!module/(.*?)/(.*)!", $relpath, $matches))
      {
        return moduleDir($matches[1]).$matches[2];
      }
      
      if      (substr($relpath,0,4)==='atk/')    $location='atk';
      else if (substr($relpath,0,7)==='themes/') $location='app';

      return ($location==='app'?atkconfig('application_dir'):atkconfig("atkroot")).$relpath;
    }

    /**
     * Load the theme information into memory.
     *
     * If a cached file with theme information doesn't exist, it is compiled
     * from the theme dir.
     */
    function _loadTheme()
    {
      if (!count((array)$this->m_theme))
      {
        $filename = atkconfig("atktempdir")."themes/".$this->m_name.".inc";
        if (!file_exists($filename) || atkconfig("force_theme_recompile"))
        {
           $compiler = atknew("atk.ui.atkthemecompiler");
           $compiler->compile($this->m_name);
        }
        include($filename);
        $this->m_theme = $theme; // $theme is set by compiled file
      }
    }

    /**
     * Returns the value for themevalue
     * Example: getAttribute("highlight");
     *          returns "#eeeeee"
     * @param string $attribname the name of the attribute in the themedefinition
     * @param string $default the default to fall back on
     * @return var the value of the attribute in the themedefinition
     */
    function getAttribute($attribname,$default="")
    {
      return (isset($this->m_theme["attributes"][$attribname])?$this->m_theme["attributes"][$attribname]:$default);
    }

    /**
     * Retrieve the location of a file
     * @access private
     * 
     * @param string $type the type of the file
     * @param string $name the name of the themefile
     * @param string $module the name of the module requesting the file
     */
    function getFileLocation($type, $name, $module="")
    {
      if ($module!="" && isset($this->m_theme["modulefiles"][$module][$type][$name]))
      {
        return moduleDir($module)."themes/".$this->m_theme["modulefiles"][$module][$type][$name];
      }
      else if (isset($this->m_theme["files"][$type][$name]))
      {
        return atkTheme::absPath($this->m_theme["files"][$type][$name]);
      }
      return "";
    }


    /**
     * Returns full path for themed template file
     * @param string $tpl the template name
     * @param string $module the name of the module requesting the file
     * @return string the full path of the template file
     */
    function tplPath($tpl, $module="")
    {
      return $this->getFileLocation("templates", $tpl, $module);
    }

    /**
     * Returns full path for themed image file
     * @param string $img the image name
     * @param string $module the name of the module requesting the file
     * @return string the full path of the image file
     */
    function imgPath($img, $module="")
    {
      $exts = array('', '.png', '.gif', '.jpg');
      
      foreach ($exts as $ext)
      {      
        $location = $this->getFileLocation("images", $img.$ext, $module);
        if ($location != null)
        {
          return $location;
        }
      }
      
      return null;
    }

    /**
     * Returns full path for themed style file
     * @param string $style the name of the CSS file
     * @param string $module the name of the module requesting the file
     * @return string the full path of the style file
     */
    function stylePath($style, $module="")
    {
      return $this->getFileLocation("styles", $style, $module);
    }

    /**
     * Returns full path for themed icon file
     * @param string $icon   the icon name (no extension)
     * @param string $type   the icon type (example: "recordlist")
     * @param string $module the name of the module requesting the file
     * @param string $ext    the extension of the file,
     *                       if this is empty, atkTheme will check several
     *                       extensions.
     * @param boolean $useDefault use default icon fallback if not found?
     * @return string the full path of the icon file
     */
    function iconPath($icon, $type, $module="",$ext='',$useDefault=true)
    {
      // Check module themes for icon
      $iconfile = $this->getIconFileFromModuleTheme($icon,$type,$ext);
      if ($module!="" && $iconfile) return moduleDir($module)."themes/".$iconfile;

      // Check the default theme for icon
      $iconfile = $this->getIconFileFromTheme($icon,$type, $this->m_theme['files'],$ext);
      if ($iconfile) return atkTheme::absPath($iconfile);

      if ($useDefault)
      {
        // Check the default theme for default icon
        $iconfile = $this->getIconFileFromTheme('default',$type,$this->m_theme['files'],$ext);
        if ($iconfile) return atkTheme::absPath($iconfile);
      }

      return false;
    }

    /**
     * Get the icon file from the module theme
     *
     * @param string $icon the name of the icon
     * @param string $type the type of the icon
     * @param string $ext the file extension
     * @return String iconfile
     */
    function getIconFileFromModuleTheme($icon,$type,$ext="")
    {
      if (!isset($this->m_theme['modulefiles']))
        return false;
      $modules = atkGetModules();
      $modulenames = array_keys($modules);
      foreach ($modulenames as $modulename)
      {
        if (isset($this->m_theme['modulefiles'][$modulename]))
        {
          $iconfile = $this->getIconFileFromTheme($icon,$type, $this->m_theme['modulefiles'][$modulename],$ext);
          if ($iconfile) return $iconfile;
        }
      }
      return false;
    }

    /**
     * Get the icon file from the theme
     *
     * @param string $iconname the name of the icon
     * @param string $type the type of the icon
     * @param array $theme the theme array containing all files
     * @param string $ext the file extension
     * @return String iconfile
     */
    function getIconFileFromTheme($iconname, $type, $theme,$ext="")
    {
      if ($ext) return $this->_getIconFileWithExtFromTheme($iconname,$ext,$type,$theme);

      $allowediconext=array('png','gif','jpg');
      foreach ($allowediconext as $ext)
      {
        $iconfile = $this->_getIconFileWithExtFromTheme($iconname,$ext,$type,$theme);
        if ($iconfile) return $iconfile;
      }
      return false;
    }

    /**
     * Get the icon file from this theme
     *
     * @param string $iconname the iconname
     * @param string $ext the file extension
     * @param string $type the icon type
     * @param array $theme the theme array containing all files
     * @return String iconfile
     */
    function _getIconFileWithExtFromTheme($iconname,$ext,$type,$theme)
    {
      if (isset($theme['icons'][$type][$iconname.".".$ext]))
      {
        $iconfile = $theme['icons'][$type][$iconname.".".$ext];
        if ($iconfile) return $iconfile;
      }
      return false;
    }

    /**
     * Gets the directory of the current theme
     * @return string full path of the current theme
     */
    function themeDir()
    {
      return atkTheme::absPath($this->getAttribute("basepath"));
    }
  }
?>
