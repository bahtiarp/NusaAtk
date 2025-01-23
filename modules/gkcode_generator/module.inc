<?php
  /**
   * The module definition class.
   */
  class mod_gkcode_generator extends atkModule
  {           
    function getMenuItems()
    {          
          $menu_label = "ATK Code Generator";
          $module = "gkcode_generator";
          // parent menu
		$this->menuitem($menu_label); 		
			$this->menuitem("Code Generator<hr>", dispatch_url($module.".code_generator", "intro"), $menu_label); 	
    }// end mod

    function getNodes()
    {    
      /**
       * In this case, we tell the system that the nodes
       * have 'admin', 'add', 'edit' and 'delete' privileges.
       */
	registerNode($this->module.".code_generator", array("admin", "add", "edit", "delete"));

    }
        
  }
?>
