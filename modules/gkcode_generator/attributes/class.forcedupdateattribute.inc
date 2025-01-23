<?php

/** 
 * The forcedupdate attribute 
 * This attribute is like a normal atkAttribute only it is always updated 
 */ 
class forcedUpdateAttribute extends atkAttribute 
{ 
  function forcedUpdateAttribute($name, $flags=0, $size=0) 
  { 
    return $this->atkAttribute($name, $flags, $size); 
  } 

  /** 
   * We always update this attribute 
   * @return bool Wether or not to update the field in the query 
   */ 
  function needsUpdate() 
  { 
    return true;
  } 
} 
?>
