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
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 6289 $
   * $Id: class.auth_db.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * Driver for authentication and authorization using tables in the database.
   *
   * @author Ivo Jansch <ivo@achievo.org>
   * @package atk
   * @subpackage security
   *
   */
   class auth_db extends auth_interface
   {
     var $m_rightscache = array();

     /**
      * Build the query for selecting the user for authentication
      *
      * @param string $user
      * @param string $usertable
      * @param string $userfield
      * @param string $passwordfield
      * @param string $accountdisablefield
      * @param string $accountenbleexpression
      * @return string which contains the query
      */
     function buildSelectUserQuery($user,$usertable,$userfield,$passwordfield,$accountdisablefield=null,$accountenbleexpression=null)
     {
       $disableexpr="";
       if($accountdisablefield) $disableexpr = ", $accountdisablefield";
       $query = "SELECT $passwordfield $disableexpr FROM $usertable WHERE $userfield ='$user'";
       if ($accountenbleexpression) $query .= " AND $accountenbleexpression";
       return $query;
     }

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

       $db = atkGetDb(atkconfig("auth_database"));
       $query = $this->buildSelectUserQuery($db->escapeSql($user),
                                            atkconfig("auth_usertable"),
                                            atkconfig("auth_userfield"),
                                            atkconfig("auth_passwordfield"),
                                            atkconfig("auth_accountdisablefield"),
                                            atkconfig("auth_accountenableexpression"));
       $recs = $db->getrows($query);
       if (count((array)$recs)>0 && $this->isLocked($recs[0]))
       {
         return AUTH_LOCKED;
       }

       return ((count((array)$recs)>0 && $user!="" && $this->matchPasswords($this->getPassword($recs[0]), $passwd))?AUTH_SUCCESS:AUTH_MISMATCH);
     }

     /**
      * returns the users password from record
      *
      * @param array $rec record from database
      * @return mixed userspw
      */
     function getPassword($rec)
     {
       return (isset($rec[atkconfig("auth_passwordfield")]))?$rec[atkconfig("auth_passwordfield")]:false;
     }

     /**
      * checks wether the useraccount is locked
      *
      * @param array $rec record from db
      * @return bool true in case of a locked account
      */
     function isLocked($rec)
     {
       return (isset($rec[atkconfig("auth_accountdisablefield")])&&$rec[atkconfig("auth_accountdisablefield")]==1);
     }

     /**
      * Match 2 passwords.
      * In normal situations, $dbpassword and $userpasswd are considered equal
      * if they are a case-insensitive match. When $config_auth_cryptedpassword
      * is true, they are only considered a match if $dbpassword is equal to the
      * crypt() of $userpasswd, where $dbpassword itself is used as the 'salt'.
      * (This method is used by Bugzilla, among other apps)
      * 
      * @param string $dbpasswd The password from the database
      * @param string $userpasswd The password the user provided
      * @return boolean which indicates if the passwords are equal
      */
     function matchPasswords($dbpasswd, $userpasswd)
     {
       // crypt password method, like in bugzilla
       if (atkconfig("auth_usecryptedpassword", false))
       {
       	 // password is stored using the crypt method, using the cryptedpassword itself as the salt.
       	 return (crypt($userpasswd, $dbpasswd)==$dbpasswd);
       }
       else
       {
       	 // regular match, perhaps with md5.
     	   return (strtoupper($dbpasswd)==strtoupper($userpasswd));
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
      * Select the user record from the database
      *
      * @param string $user
      * @return array with user information
      */
     function selectUser($user)
     {
       $usertable = atkconfig("auth_usertable");
       $userfield = atkconfig("auth_userfield");
       $leveltable = atkconfig("auth_leveltable");
       $levelfield = atkconfig("auth_levelfield");
       $userpk = atkconfig("auth_userpk");
       $userfk = atkconfig("auth_userfk", $userpk);
       $grouptable = atkconfig("auth_grouptable");
       $groupfield = atkconfig("auth_groupfield");
       $groupparentfield = atkconfig("auth_groupparentfield");
       $accountenableexpression = atkconfig("auth_accountenableexpression");

       $db = atkGetDb(atkconfig("auth_database"));
       if ($usertable==$leveltable||$leveltable=="")
       {
         // Level and userid are stored in the same table.
         // This means one user can only have one level.
         $query = "SELECT * FROM $usertable WHERE $userfield ='$user'";
       }
       else
       {
         // Level and userid are stored in two separate tables. This could
         // mean (but doesn't have to) that a user can have more than one
         // level.
	       $qryobj =&$db->createQuery();
	       $qryobj->addTable($usertable);
	       $qryobj->addField("$usertable.*");
	       $qryobj->addField("usergroup.*");
	       $qryobj->addJoin($leveltable, "usergroup", "$usertable.$userpk = usergroup.$userfk", true);
	       $qryobj->addCondition("$usertable.$userfield = '$user'");

	       if (!empty($groupparentfield))
	       {
	         $qryobj->addField("grp.$groupparentfield");
	         $qryobj->addJoin($grouptable, "grp", "usergroup.$levelfield = grp.$groupfield", true);
	       }
	       $query = $qryobj->buildSelect();
       }

       if ($accountenableexpression) $query .= " AND $accountenableexpression";
       $recs = $db->getrows($query);
       return $recs;
     }

     /**
      * Get the parent groups 
      *
      * @param array $parents
      * @return array with records of the parent groups
      */
     function getParentGroups($parents)
     {
       $db = atkGetDb(atkconfig("auth_database"));

       $grouptable = atkconfig("auth_grouptable");
       $groupfield = atkconfig("auth_groupfield");
       $groupparentfield = atkconfig("auth_groupparentfield");

       $query = &$db->createQuery();
       $query->addField($groupparentfield);
	     $query->addTable($grouptable);
       $query->addCondition("$grouptable.$groupfield IN (".implode(',', $parents).")");
       $recs = $db->getrows($query->buildSelect(TRUE));
       return $recs;
     }
     
     /** 
     * This function returns information about a user in an associative
     * array with the following elements (minimal):
     * "name" -> the userid (should normally be the same as the $user 
     *           variable that gets passed to it.
     * "level" -> The level/group(s) to which this user belongs.
     * "groups" -> The groups this user belongs to
     * "access_level" -> The user's access level
     * The other elemens of the returning array depend on the structure of
     * the user table
     *
     * @param String $user The login of the user to retrieve.
     * @return array Information about a user.
     */
     function getUser($user)
     {
       $grouptable = atkconfig("auth_grouptable");
       $groupfield = atkconfig("auth_groupfield");
       $groupparentfield = atkconfig("auth_groupparentfield");

       $recs = $this->selectUser($user);
       $groups = array();
       $level = array();

       // We might have more then one level, so we loop the result.
       if (count((array)$recs)>0)
       {
         $level = array();
         $parents = array();

         for ($i=0;$i<count((array)$recs);$i++)
         {
           $level[] = $recs[$i][atkconfig("auth_levelfield")];
           $groups[] = $recs[$i][atkconfig("auth_levelfield")];

           if (!empty($groupparentfield) && $recs[$i][$groupparentfield] != "")
             $parents[] = $recs[$i][$groupparentfield];
         }

         $groups = array_merge($groups, $parents);
         while (count((array)$parents) > 0)
         {
           $precs = $this->getParentGroups($parents);
           $parents = array();
  	       foreach ($precs as $prec)
  	         if ($prec[$groupparentfield] != "")
  	           $parents[] = $prec[$groupparentfield];

           $groups = array_merge($groups, $parents);
         }

         $groups = array_unique($groups);
       }
       if (count((array)$level) == 1)
         $level = $level[0];

       $userinfo = $recs[0];
       $userinfo["name"]=$user;
       $userinfo["level"]=$level; // deprecated. But present for backwardcompatibility.
       $userinfo["groups"]=$groups;
       $userinfo["access_level"]=$this->getAccessLevel($recs);

       return $userinfo;
     }

     /**
      * Get the access level from the user
      *
      * @param array $recs The records that are returned by the selectUser function
      * @return the access level
      */
     function getAccessLevel($recs)
     {
       // We might have more then one access level, so we loop the result.
       if (count((array)$recs)>1)
       {
         $access = array();
         for ($i=0;$i<count((array)$recs);$i++)
         {
           if($i==0) $access = $recs[$i][atkconfig("auth_accesslevelfield")];
           if($recs[$i][atkconfig("auth_accesslevelfield")]>$access) $access = $recs[$i][atkconfig("auth_accesslevelfield")];
         }
       }
       else
       {
         $access = "";
         if (isset($recs[0][atkconfig("auth_accesslevelfield")]))
         {
           $access = $recs[0][atkconfig("auth_accesslevelfield")];
         }
       }
       return $access;
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
       $db = atkGetDb(atkconfig("auth_database"));

       if (!isset($this->m_rightscache[$node])||count((array)$this->m_rightscache[$node])==0)
       {
         $query = "SELECT * FROM ".atkconfig("auth_accesstable")." WHERE node='$node'";

         $this->m_rightscache[$node] = $db->getrows($query);
       }

       $result = Array();

       $rights = $this->m_rightscache[$node];

       $field = atkconfig("auth_accessfield");
       if (empty($field))
         $field = atkconfig("auth_levelfield");

       for ($i=0, $_i=count((array)$rights); $i<$_i; $i++)
       {
         if ($rights[$i]['action'] == $action)
         {
	         $result[] = $rights[$i][$field];
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
       $db = atkGetDb(atkconfig("auth_database"));

       $query = "SELECT * FROM attribaccess WHERE node='$node' AND attribute='$attrib' AND mode='$mode'";

       $rights = $db->getrows($query);

       $result = Array();

       for ($i=0;$i<count((array)$rights);$i++)
       {
         if ($rights[$i][atkconfig("auth_levelfield")]!="")
         {
           $result[] = $rights[$i][atkconfig("auth_levelfield")];
         }
       }

       return $result;
     }

     /**
      * Compare function for sorting users on their username
      *
      * @param array $a
      * @param array $b
      * @return boolean 
      */
     function userListCompare($a, $b)
     {
     	 return strcmp($a["username"], $b["username"]);
     }

     /**
      * This function returns the list of users that may login. This can be
      * used to display a dropdown of users from which to choose.
      *
      * @return array List of users as an associative array with the following 
      *               format: array of records, each record is an associative
      *               array with a userid and a username field.
      */
     function getUserList()
     {
       $db = atkGetDb(atkconfig("auth_database"));
       $query = "SELECT * FROM ".atkconfig("auth_usertable");

       $accountdisablefield = atkconfig("auth_accountdisablefield");
       $accountenableexpression = atkconfig("auth_accountenableexpression");
       if ($accountenableexpression!="")
       {
         $query.= " WHERE $accountenableexpression";
         if ($accountdisablefield!="") $query.= " AND $accountdisablefield = 0";
       }
       else
       {
         if ($accountdisablefield!="") $query.= " WHERE $accountdisablefield = 0";
       }

       $recs = $db->getrows($query);

       $userlist = array();
       atkimport("atk.utils.atkstringparser");
       $stringparser = new atkStringParser(atkconfig("auth_userdescriptor"));
       for ($i=0, $_i=count((array)$recs); $i<$_i; $i++)
       {
         $userlist[] = array("userid"=>$recs[$i][atkconfig("auth_userfield")], "username"=>$stringparser->parse($recs[$i]));
       }
       usort($userlist, array("auth_db", "userListCompare"));
       return $userlist;
     }
     
     /**
      * This function returns a boolean that is true when the class allows the
      * resetting of the password of a user.
      * @deprecated Seems like this function is not used anymore
      *
      * @return true
      */
     function setPasswordAllowed()
     {
       return true;
     }
   }

?>
