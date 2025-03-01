<?php
/**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage security
   *
   * @copyright (c)2000-2004 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6289 $
   * $Id: class.auth_config.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * Driver for authentication and authorization using entries in the
   * configurationfile.
   *
   * See the methods in the atkConfig class for an explanation of how to add
   * users and privileges.
   *
   * Does not support authorization.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage security
   *
   */
   class auth_config extends auth_interface
   {
     /**
     * Authenticate a user. 
     *
     * @param String $user The login of the user to authenticate.
     * @param String $passwd The password of the user. Note: if the canMd5 
     *                       function of an implementation returns true,      
     *                       $passwd will be passed as an md5 string.
     *
     * @return int AUTH_SUCCESS - Authentication succesful
     *             AUTH_MISMATCH - Authentication failed, wrong 
     *                             user/password combination
     *             AUTH_LOCKED - Account is locked, can not login
     *                           with current username.
     *             AUTH_ERROR - Authentication failed due to some 
     *                          error which cannot be solved by 
     *                          just trying again. If you return 
     *                          this value, you *must* also 
     *                          fill the m_fatalError variable.
     */
     function validateUser($user, $passwd)
     {
       if ($user=="") return AUTH_UNVERIFIED; // can't verify if we have no userid

       global $config_user;
       if ($user!="" && $passwd!="" && $config_user[$user]["password"]==$passwd)
       {
         return AUTH_SUCCESS;
       }
       else
       {
         return AUTH_MISMATCH;
       }
     }

     /**
     * Does the authentication method support md5 encoding of passwords?
     *
     * @return boolean True if md5 is always used. false if md5 is not
     *                 supported.
     *                 Drivers that support both md5 and cleartext passwords
     *                 can return atkconfig("authentication_md5") to let the
     *                 application decide whether to use md5.
     */ 
     function canMd5()
     {
       return atkconfig("authentication_md5");
     }

     /** 
     * This function returns information about a user in an associative
     * array with the following elements:
     * "name" -> the userid (should normally be the same as the $user 
     *           variable that gets passed to it.
     * "level" -> The level/group(s) to which this user belongs.
     * Specific implementations of the method may add more information if
     * necessary.
     *
     * @param String $user The login of the user to retrieve.
     * @return array Information about a user.
     */
     function getUser($user)
     {
       global $config_user;
       return Array("name"=>$user, "level"=>$config_user[$user]["level"]);
     }

     /**
     * This function returns the level/group(s) that are allowed to perform
     * the given action on a node.
     * @param String $node The full nodename of the node for which to check
     *                     the privilege. (modulename.nodename)
     * @param String $action The privilege to check.
     * @return mixed One (int) or more (array) entities that are allowed to
     *               perform the action.
     */
     function getEntity($node, $action)
     {
       global $config_access;

       $rights = $config_access[$node];

       $result = Array();

       for ($i=0;$i<count((array)$rights);$i++)
       {
         if ($rights[$i][$action]!="")
         {
           $result[] = $rights[$i][$action];
         }
         if ($rights[$i]["*"]!="")
         {
           $result[] = $rights[$i]["*"];
         }
       }

       return $result;
     }

     /** 
     * This function returns the level/group(s) that are allowed to 
     * view/edit a certain attribute of a given node.
     * @param String $node The full nodename of the node for which to check
     *                     attribute access.
     * @param String $attrib The name of the attribute to check
     * @param String $mode "view" or "edit"
     * @param mixed One (int) or more (array) entities that are allowed to 
     *              view/edit the attribute. 
     */
     function getAttribEntity($node, $attrib, $mode)
     {
       global $config_attribrestrict;

       // $entity is an array of entities that may do $mode on $node.$attrib.
       $entity = $config_attribrestrict[$node][$attrib][$mode];

       return $entity;
     }
     
     /**
      * This function returns a boolean that is true when the class allows the
      * resetting of the password of a user.
      * @deprecated Seems like this function is not used anymore
      *
      * @return false
      */
     function setPasswordAllowed()
     {
       return false;
     }
   }

?>
