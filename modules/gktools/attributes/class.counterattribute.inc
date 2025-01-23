<?php
atkimport("atk.attributes.atkdummyattribute"); 
class counterAttribute extends atkDummyAttribute 
{ 
   function counterAttribute($name, $flags=0) 
   { 
      $this->atkDummyAttribute($name, '', $flags|AF_HIDE_VIEW|AF_HIDE_EDIT|AF_HIDE_ADD); 
   } 

   function display($record, $mode = "")  
   { 
      static $s_counter = 0; 
      $node = &$this->m_ownerInstance; 
      return $node->m_postvars["atkstartat"]+(++$s_counter); 
   } 
} 

?>
