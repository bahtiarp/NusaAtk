<?PHP
  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage menu
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6347 $
   * $Id: class.atkdropdownmenu.inc 6589 2009-12-01 10:13:19Z peter $
   */
  atkimport('atk.menu.atkplainmenu');

  /**
   * Implementation of the Dropdowntext menu.
   *
   * @author Ber Dohmen <ber@ibuildings.nl>
   * @author Sandy Pleyte <sandy@ibuildings.nl>
   * @package atk
   * @subpackage menu
   */
  class atkDropdownMenu extends atkPlainMenu
  {
    /**
	   * Render the menu
	   * @return String HTML fragment containing the menu.
	   */
    function render()
    {
      $page = atkinstance("atk.ui.atkpage");
      $menu = $this->load();
      $page->addContent($menu);

      return $page->render("Menu", true);
    }

    /**
     * Get the menu
     *
     * @return string The menu
     */
    function getMenu() { return $this->load(); }
    
    /**
     * Load the menu
     *
     * @return string The menu
     */
    function load()
    {
      global $ATK_VARS,$g_menu;

      $page = atkinstance('atk.ui.atkpage');
      $theme = atkinstance("atk.ui.atktheme");
      $page->register_script(atkconfig('atkroot').'atk/javascript/dropdown_menu.js');
      $page->register_style($theme->stylePath("atkdropdownmenu.css"));
      $page->m_loadscripts[] = "new DHTMLListMenu('nav');";

      $atkmenutop = array_key_exists('atkmenutop',$ATK_VARS)?$ATK_VARS["atkmenutop"]:'main';
      if (!is_array($g_menu[$atkmenutop]))
        $g_menu[$atkmenutop] = array();
      usort($g_menu[$atkmenutop],array("atkplainmenu","menu_cmp"));

      $menu= "<div id=\"nav\">\n";
      $menu.=$this->getHeader($atkmenutop);

      $menu.="  <ul>\n";
      foreach ($g_menu[$atkmenutop] as $menuitem)
      {
        $menu .= $this->getMenuItem($menuitem, "    ");
      }
      
      if (atkconfig("menu_logout_link"))
      {
        $menu.="    <li><a href=\"app.php?atklogout=1\">".atktext('logout')."</a></li>\n";
      }
      
      $menu.="  </ul>\n";

      $menu.=$this->getFooter($atkmenutop);
      $menu.="</div>\n";
      return $menu;
    }

    /**
     * Get a menu item
     *
     * @param string $menuitem
     * @param string $indentation
     * @return string The menu item
     */
    function getMenuItem($menuitem, $indentation="")
    {
      global $g_menu;
      $enable = $this->isEnabled($menuitem);
      $menu = '';
      if ($enable)
      {
        if (array_key_exists($menuitem['name'],$g_menu) && $g_menu[$menuitem['name']])
        {
          $submenu=$indentation."<ul>\n";
          foreach ($g_menu[$menuitem['name']] as $submenuitem)
          {
            $submenu.=$this->getMenuItem($submenuitem,$indentation."  ",$submenuname='',$menuitem['name']);
          }
          $submenu.=$indentation."</ul>\n";
          $menu.=$indentation.$this->getItemHtml($menuitem, "\n".$submenu.$indentation);
        }
        else
        {
          $menu.=$indentation.$this->getItemHtml($menuitem);
        }
      }
      return $menu;
    }

    /**
     * Get the HTML for a menu item
     *
     * @param string $menuitem
     * @param string $submenu
     * @param string $submenuname
     * @return string The HTML for a menu item
     */
    function getItemHtml($menuitem, $submenu="",$submenuname='')
    {
      $delimiter = atkconfig("menu_delimiter");

      $name = $this->getMenuTranslation($menuitem['name'],$menuitem['module']);
      if ($menuitem['name']=='-') return "<li class=\"separator\"><div></div></li>\n";
      if ($menuitem['url'] && substr($menuitem['url'], 0, 11) == 'javascript:') 
      {
        $href = '<a href="javascript:void(0)" onclick="'.atk_htmlentities($menuitem['url']).'; return false;">'.atk_htmlentities($this->getMenuTranslation($menuitem['name'],$menuitem['module'])).'</a>'; 
      }
      else if ($menuitem['url'])
      {
        $href = href($menuitem['url'],$this->getMenuTranslation($menuitem['name'],$menuitem['module']), SESSION_NEW);
      }
      else $href = '<a href="#">'.$name.'</a>';

      return "<li id=\"{$menuitem['module']}.{$menuitem['name']}\" class=\"$submenuname\">".$href.$delimiter.$submenu."</li>\n";
    }
  }

?>
