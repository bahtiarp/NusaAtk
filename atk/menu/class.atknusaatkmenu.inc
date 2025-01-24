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
  class atkNusaatkMenu extends atkPlainMenu
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
      $ui = atkinstance("atk.ui.atkui");
      $icon = array(
            "fa-solid fa-chart-simple",
            "fa-laptop",
            "fa-edit",
            "fa-table",
            "fa-calendar",
            "fa-envelope",
            "fa-folder",
            "fa-dashboard",
            "fa-th"
        );



      $custom_icon = array(
        "whatsapp_business" => "fa-whatsapp",
      );
      
      $i=0;

      $atkmenutop = array_key_exists('atkmenutop',$ATK_VARS)?$ATK_VARS["atkmenutop"]:'main';
      if (!is_array($g_menu[$atkmenutop]))
        $g_menu[$atkmenutop] = array();
      usort($g_menu[$atkmenutop],array("atkplainmenu","menu_cmp"));

      
    //  $menu.="<div class=\"collapse navbar-collapse\" id=\"bs-example-navbar-collapse-1\">";
      $menu="";
      foreach ($g_menu[$atkmenutop] as $menuitem)
      {
        $ico = isset($custom_icon[$menuitem["name"]]) ? $custom_icon["whatsapp_business"] : $icon[$i];
        $menu .= $this->getMenuItem($menuitem, "",$ico);
        $i++;
        if($i==8){$i=0;}
      }


      $user=getUser();
      // var_dump($user);exit;
      if($user["name"=="administrator"]){$user["fullname"]="Administrator";}



      $user["is_whitelabel"] = 0;
      $user["parent_whitelabel"] = 0;
      $user["firstname"] = "Admin";
      $user["lastname"] = "Demo";
      $user["clientName"] = "PT. Visio Informatika";
      $user["idClient"] = "1";
      $user["idClientParent"] = "1";
      $user["contract_type"] = "1";
      $user["invoice"] = 1;
      $user["isAdmin"] = "1";
      $user["idPerson"] = "1";
      $user["person_img"] = "https://app.nusasms.com/images/person/munjul-4339.jpeg";
   
       $content = $ui->render("./themes/nusaatk/templates/menu.tpl", array("content" =>$menu,"fulluser"=>$user,"invoice"  => $user["invoice"]));
    
      return $content;
    }

    /**
     * Get a menu item
     *
     * @param string $menuitem
     * @param string $indentation
     * @return string The menu item
     */
    function getMenuItem($menuitem, $indentation="",$icon)
    {
      global $g_menu;
      $enable = $this->isEnabled($menuitem);
      $menu = '';

      $icons = array(
        "bi bi-bar-chart",
        "bi bi-laptop",
        "bi bi-credit-card",
        "bi bi-table",
        "bi bi-calendar-day",
        "bi bi-envelope-arrow-up",
        "bi bi-folder",
        "bi bi-bank",
        "bi bi-browser-chrome"
    );

      $custom_icon = array(
        "phone_number" => "fa-sign-in",
        "templates" => "fa-file-text-o",
        "broadcast" => "fa-commenting-o",
        "outbox" => "fa-inbox",
        "inbox" => "fa-inbox",
        "download_report" => "fa-download",
      );

      $i=0;
        
      if ($enable)
      {
        if (array_key_exists($menuitem['name'],$g_menu) && $g_menu[$menuitem['name']])
        {
            
    //      $submenu=$menuitem['name'].$indentation."<ul class=\"dropdown-menu\">\n";
          $submenu=$indentation."<ul   class=\"nav nav-treeview\">\n";
          foreach ($g_menu[$menuitem['name']] as $submenuitem)
          {
        //    $submenu.=$this->getMenuItem($submenuitem,$indentation."  ",$submenuname='',$menuitem['name']);
            $ico = isset($custom_icon[$submenuitem["name"]]) ? $custom_icon[$submenuitem["name"]] : $icons[$i];
            $submenu.=$this->getMenuItem($submenuitem,$indentation."  ",$ico);
                    $i++;
          if($i==8)$i=0;
          }
          $submenu.=$indentation."</ul>\n";
          $menu.=$indentation.$this->getItemHtml($menuitem, "\n".$submenu.$indentation,$icon);
  
        }
        else
        {
          $menu.=$indentation.$this->getItemHtml($menuitem, "\n"."",$icon);
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
    function getItemHtml($menuitem, $submenu="",$icon='dropdown')
    {
     // $delimiter = atkconfig("menu_delimiter");
      $delimiter = "";

      $name = $this->getMenuTranslation($menuitem['name'],$menuitem['module']);
      if ($menuitem['name']=='-') return "<li class=\"separator\"><div></div></li>\n";
      if ($menuitem['url'] && substr($menuitem['url'], 0, 11) == 'javascript:') 
      {
        $href = '<a href="javascript:void(0)" onclick="'.atk_htmlentities($menuitem['url']).'; return false;">'.atk_htmlentities($this->getMenuTranslation($menuitem['name'],$menuitem['module'])).'</a>'; 
      }
      else if ($menuitem['url'])
      {
        // $href = href($menuitem['url'],"<i color=\"darkblue\" class=\"fa $icon\"></i><p>".$this->getMenuTranslation($menuitem['name'],$menuitem['module']), SESSION_NEW,false,"class=\"nav-link\"")."</p>";
     
        // $href = '<a href="'.$menuitem['url'].'" class="nav-link"> <i class="nav-icon bi bi-bar-chart"></i> <p>'.$this->getMenuTranslation($menuitem['name'],$menuitem['module']).'</p></a>';
        $href = '<a href="'.$menuitem['url'].'" class="nav-link"> <i class="nav-icon '.$icon.'"></i> <p>'.$this->getMenuTranslation($menuitem['name'],$menuitem['module']).'</p></a>';

        $ret="<li  class=\"nav-item\" id=\"{$menuitem['module']}.{$menuitem['name']}\" >".$href.$submenu."</li>\n";
        }
      else {
          $href = '<a href="#" class="nav-link" >'."<i color=\"darkblue\" class=\"fa $icon\"></i><p>".$name.'<i style="font-size: smaller;" class="nav-arrow bi bi-chevron-right"></i></p></a>';
          $ret="<li  id=\"{$menuitem['module']}.{$menuitem['name']}\" class=\"nav-item\">".$href.$submenu."</li>\n";
          
      }
        return $ret;
    }
  }

?>
