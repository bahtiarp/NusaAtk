<?php
  
  /**
   * The module definition class.
   *
   * This module introduces, among other things, security profiles. Employees
   * get a login name/password, and a profile.
   */
  class mod_gktools extends atkModule
  {           
    
    /**
     * This lesson contains security profiles. This means that no longer all
     * users may execute all actions. The administrator user still can, but
     * other users need privileges.
     * With this method, the system is instructed of the privileges that
     * exist. Usually, this corresponds to the actions that can be done
     * on a node.
     */
    function getNodes()
    {    
      /**
       * In this case, we tell the system that the employee node of the
       * company module has 'admin', 'add', 'edit' and 'delete' privileges.
       */
      registerNode("gktools.gkimagetools", array("admin", "add", "edit", "delete"));
      
      /** 
       * If a user has the grantall privilege, he can grant other users all 
       * privileges; even privileges he does not have himself. Without this
       * privilege, users can only grant rights to other users that they have
       * themselves.
       */
      //registerNode("company.profile", array("admin", "add", "edit", "delete", "grantall"));
      //registerNode("company.department", array("admin", "add", "edit", "delete"));
    }
    
  }

?>
