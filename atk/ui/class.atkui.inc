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
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6309 $
   * $Id: class.atkui.inc 6715 2010-02-10 18:26:57Z mvdvelden $
   */

  /**
   * Utility class for rendering boxes, lists, tabs or other templates.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage ui
   */
  class atkUI
  {
    /**
     * Smarty instance, initialised by constructor
     * @access private
     * @var Smarty
     */
    var $m_smarty = NULL;

    /**
     * atkTheme instance, initialised by constructor
     * @access private
     * @var atkTheme
     */
    var $m_theme = NULL;

    /**
     * atkUi constructor, initialises Smarty and atkTheme instance
     */
    function __construct()
    {
      $this->atkUI();
    }
    function atkUI()
    {
      $this->m_theme = atkinstance("atk.ui.atktheme");
      $this->m_smarty = atkinstance("atk.ui.atksmarty");
    }

    /**
     * get a singleton instance of the atkUi class.
     *
     * @return atkUI
     */
    static public function &getInstance()
    {
      static $s_instance = NULL;

      if ($s_instance==NULL)
      {
        atkdebug("Creating a new atkUI instance");
        $s_instance = new atkUi();
      }

      return $s_instance;
    }

    /**
     * Renders action templates
     * Currently only the view action is implemented
     * @param String $action the action for which to render the template
     * @param array  $vars   the template variables
     * @param string $module the name of the module requesting to render a template
     * @return String the rendered template
     */
    function renderAction($action, $vars, $module="")
    {
      // todo.. action specific templates
      $tpl = "action_$action.tpl";
      if ($this->m_theme->tplPath($tpl)=="") // no specific theme for this action
      {
      	
        $tpl = "action.tpl";
      }
      return $this->render($tpl, $vars, $module);
    }

    /**
     * Renders a list template
     * @param String $action not used (deprecated?)
     * @param array  $vars   the variables with which to parse the list template
     * @param string $module the name of the module requesting to render a template 
     */
    function renderList($action, $vars, $module="")
    {
      return $this->render("list.tpl", $vars, $module);
    }

    /**
     * Renders top template
     * @deprecated Use renderBox($vars, "top") instead
     * @param array $vars the variables for the template
     * @param string $module the name of the module requesting to render a template
     * @return String with the rendered box
     */
    function renderTop($vars, $module="")
    {
      atkdebug("Using deprecated renderTop function, please install newer top.php from the atk/skel");
      return $this->renderBox($vars, "top", $module);
    }

    /**
     * Renders a box with Smarty template.
     * Call with a $name variable to provide a
     * better default than "box.tpl".
     *
     * For instance, calling renderBox($smartyvars, "menu")
     * will make it search for a menu.tpl first and use that
     * if it's available, otherwise it will just use box.tpl
     * 
     * @param array $vars the variables for the template
     * @param string $name The name of the template
     * @param string $module the name of the module requesting to render a template
     */

    function renderBox($vars, $name="", $module="")
    {
      if ($name && file_exists($this->m_theme->tplPath($name.".tpl",$module)))
      {
        return $this->render($name.".tpl", $vars,$module);
      }      
      //var_dump($vars);
      return $this->render("box.tpl", $vars, $module);
    }
    
    /**
     * Renders the insides of a dialog.
     * 
     * @param array $vars template variables
     * @param string $module the name of the module requesting to render a template
     * @return string rendered dialog
     */
    function renderDialog($vars, $module="")
    {
      return $this->render("dialog.tpl", $vars, $module);
    }

    /**
     * Renders a tabulated template
     * Registers some scriptcode too when the tabtype is set to dhtml
     * @param array $vars the variables with which to render the template
     * @param string $module the name of the module requesting to render a template
     * @return String the rendered template
     */
    function renderTabs($vars, $module="")
    {
      if ($this->m_theme->getAttribute("tabtype")=="dhtml")
      {
        $page = atkPage::getInstance();
        $page->register_script(atkconfig("atkroot")."atk/javascript/tools.js");        
      }
      return $this->render("tabs.tpl", $vars, $module);
    }

    /**
     * Renders the given template. 
     * 
     * If the name ends with ".php" PHP will be used to render the template. If
     * the name ends with ".tpl" and a file with the extension ".tpl.php" exists
     * PHP will be used, otherwise Smarty will be used to render the template. 
     * 
     * @param String $name   the name of the template to render
     * @param array  $vars   the variables with which to render the template
     * @param String $module the name of the module requesting to render a template
     * 
     * @return String rendered template
     */
    public function render($name, $vars=array(), $module="")
    {
    	$path = $this->templatePath($name, $module);
    	
    	if (substr($path, -4) != '.php' && file_exists($path.'.php'))
    	{
    		$path .= '.php';
    	}
    	
    	if (substr($path, -4) == '.php')
    	{
    		return $this->renderPhp($path, $vars);
    	}
    	else
    	{
    		return $this->renderSmarty($path, $vars);
    	}
    }
    
    /**
     * Render PHP-based template.
     * 
     * @param string $path template path
     * @param array  $vars template variables
     * 
     * @return string rendered template
     */
    private function renderPhp($path, $vars)
    {
    	unset($path);
    	unset($vars);
    	extract(func_get_arg(1));
    	ob_start();
    	include func_get_arg(0);
    	return ob_get_clean();
    }
    
    /**
     * Render Smarty-based template.
     * 
     * @param string $path template path
     * @param array  $vars template variables
     * 
     * @return string rendered template
     */
    private function renderSmarty($path, $vars)
    {
      // First clear any existing smarty var.
      $this->m_smarty->clear_all_assign();

      // Then set some defaults that we need in all templates.
      $this->m_smarty->assign("themedir", $this->m_theme->themeDir());

      $this->m_smarty->assign("atkroot", atkconfig("atkroot"));
      $this->m_smarty->assign("application_dir", atkconfig("application_dir"));

      $this->m_smarty->assign($vars);

      // Smarty fetches templates relative from the template_dir setting.
      // Since that is an application directory, and themes reside in
      // a different directory, we have to hack the template_dir
      // setting.
      $old = $this->m_smarty->template_dir;

      // disable smarty caching for ui purposes.
      $old_caching = $this->m_smarty->caching;
      $this->m_smarty->caching = false;
      $this->m_smarty->template_dir = "./"; // current dir, because tplname already contains full relative path.
      $res = $this->m_smarty->fetch($path);
      $this->m_smarty->template_dir = $old;
      $this->m_smarty->caching = $old_caching;

      return $res;
    }

    /**
     * This function returns a complete themed path for a given template.
     * This is a convenience method, which calls the tplPath method on
     * the theme instance. However, if the template name contains a '/',
     * we assume the full template path is already given and we simply
     * return it.
     *
     * @param String $template  The filename (without path) of the template
     *                          for which you want to complete the path.
     * @param String $module    The name of the module requesting to render a template
     * @return String the template path
     */
    function templatePath($template, $module="")
    {
    
      if (strpos($template, "/")===false)
      {
        // lookup template in theme.
        $template = $this->m_theme->tplPath($template, $module);
      }
      return $template;
    }

    /**
     * This function returns a complete themed path for a given stylesheet.
     * This is a convenience method, which calls the stylePath method on
     * the theme instance.
     *
     * @param String $style The filename (without path) of the stylesheet for
     *                      which you want to complete the path.
     * @param String $module  the name of the module requesting the style path
     * @return String the path of the style
     */
    function stylePath($style, $module="")
    {
      return $this->m_theme->stylePath($style, $module);
    }
    
    /**
     * Return the title to render
     *
     * @param String $module   the module in which to look
     * @param String $nodetype the nodetype of the action
     * @param String $action   the action that we are trying to find a title for
     * @param bool   $actiononly wether or not to return a name of the node
     *                          if we couldn't find a specific title
     * @return String the title for the action
     */
    function title($module, $nodetype, $action=null, $actiononly=false)
    {
      if ($module == NULL || $nodetype == NULL) return "";
      return $this->nodeTitle(atkGetNode($module.'.'.$nodetype), $action, $actiononly);
    }

    /**
     * This function returns a suitable title text for an action.
     * Example: echo $ui->title("users", "employee", "edit"); might return:
     *          'Edit an existing employee'
     * @param atkNode $node the node to get the title from
     * @param String $action   the action that we are trying to find a title for
     * @param bool   $actiononly wether or not to return a name of the node
     *                          if we couldn't find a specific title
     * @return String the title for the action
     */
    function nodeTitle($node, $action=NULL, $actiononly=false)
    {
      if ($node == NULL) return "";

      $nodetype = $node->m_type;
      $module = $node->m_module;
      
      if ($action != NULL)
      {
        $keys = array('title_'.$module.'_'.$nodetype.'_'.$action,
                      'title_'.$nodetype.'_'.$action,
                      'title_'.$action);

        $label = $node->text($keys, NULL, "", "", true);
      }
      else
      {
        $label = "";
      }

      if ($label=="")
      {
        $actionKeys = array(
          'action_'.$module.'_'.$nodetype.'_'.$action,
          'action_'.$nodetype.'_'.$action,
          'action_'.$action,
          $action
        );
        
        if ($actiononly)
        {
          return $node->text($actionKeys);
        }
        else
        {
          $keys = array('title_'.$module.'_'.$nodetype, 'title_'.$nodetype, $nodetype);
          $label = $node->text($keys);
          if ($action != NULL)
            $label .= " - ".$node->text($actionKeys);
        }
      }
      return $label;
    }
    
    
    /**
     * This function returns a suitable title text for an Wizardpanel.
     * Example: echo $ui->title("departmentwizard", "employee", "add"); might return:
     *          'Departmen wizard - Add employees - Step 2 of 3'
     * @param String $wizard the wizard object
     * @param String $panel the panel object
     * @param String $action the atk action that we are trying execute in the panel
     * @return String the title for this wizardpanel
     */
    function getWizardTitle($wizard, $panel, $action=NULL)
    {
      if ($wizard == NULL) return "";
                            
      $module = $wizard->getModuleName();
      $wizardName = $wizard->getName();
      $panelName = $panel->getPanelName();
      
      $keys = array('title_'.$module.'_'.$wizardName,
                      'title_'.$wizardName);
      $wizardTitle = atktext($keys, NULL, "", "", true);
      
      $keys = array('title_'.$module.'_'.$panelName.'_'.$action,
                      'title_'.$panelName.'_'.$action);

      $panelTitle = atktext($keys, NULL, "", "", true);

      if($wizard->getWizardAction() !== 'finish')
        $status = atktext("Step"). " " .($wizard->m_currentPanelIndex + 1) . " " . atktext("of"). " " . count((array)$wizard->m_panelList);
      else
        $status = atktext("finished");
      $label .= $wizardTitle . " - " . $panelTitle . " - " . $status;
                                  
      return $label;
    }
  }
?>
