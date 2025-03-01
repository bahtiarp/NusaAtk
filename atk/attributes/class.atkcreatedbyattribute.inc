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
   * @copyright (c)2006 Ivo Jansch
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 4640 $
   * $Id: class.atkcreatedbyattribute.inc 6354 2009-04-15 02:41:21Z mvdam $
   */

  /**
   * @internal baseclass include
   */
  useattrib("atkupdatedbyattribute");

  /**
   * This attribute can be used to automatically store the user that inserted
   * a record.
   *
   * @author Yury Golovnya <ygolovnya@ccenter.utel.com.ua>
   * @package atk
   * @subpackage attributes
   *
   */
  class atkCreatedByAttribute extends atkUpdatedByAttribute
  {
    /**
     * Constructor.
     *
     * @param String $name Name of the field
     * @param int $flags Flags for this attribute.
     * @return atkCreatedByAttribute
     */

     function __construct($name, $flags=0)
     {
       $this->atkUpdatedByAttribute($name, $flags);
     }

    function atkCreatedByAttribute($name, $flags=0)
    {
      $this->atkUpdatedByAttribute($name, $flags);
    }

    /**
     * needsUpdate always returns false for this attribute.
     * @return false
     */
    function needsUpdate($record) { return false; }
  }
?>
