<?php

/** 
 * The fill Number Attribute
 * This attribute fills a list with the given range of numbers 
 * Author: Jorge Garifuna (GariDigital.com)
 * Created: March 7th, 2006
 * @param $name Name of the attribute
 * @param $min The minimum number of the list
 * @param $max The maximum number of the list
 * @param $flags Flags for this attribute
 * @param $size  Size of the attribute.
 * @param $incremental  The increment between each number.

 */ 
 atkimport("atk.attributes.atklistattribute");
class fillNumberAttribute extends atkListAttribute 
{ 
  function fillNumberAttribute($name, $min, $max, $flags=0, $size=0, $incremental=1) 
  { 
         /* start: order priority list */
          $optionsArray = array();
          $counter=0;
          for($i=$min; $i<=$max; $i+=$incremental){
               $optionsArray[$counter] = $i;
               $counter++;
          }               
		return $this->atkListAttribute($name,$optionsArray,$optionsArray,$flags,$size);
		/* end: order priority list */			
  } // end function
} // end class
?>
