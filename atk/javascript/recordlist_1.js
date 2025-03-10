  /**
   * This file is part of the Achievo ATK distribution.
   * Detailed copyright and licensing information can be found
   * in the doc/COPYRIGHT and doc/LICENSE files which should be
   * included in the distribution.
   *
   * @package atk
   * @subpackage javascript
   *
   * @copyright (c)2000-2004 Ibuildings.nl BV
   * @license http://www.achievo.org/atk/licensing ATK Open Source License
   *
   * @version $Revision: 5141 $
   * $Id: recordlist.js 6354 2009-04-15 02:41:21Z mvdam $
   */

function highlightrow(row, color)
{
  if (typeof(row.style) != 'undefined')
  {
    row.oldcolor = row.style.backgroundColor;
    row.style.backgroundColor = color;
  }
}

function resetrow(row)
{
  row.style.backgroundColor = row.oldcolor;
}

function selectrow(row, rlId, rownum)
{
  table = document.getElementById(rlId);
  if (table.listener && table.listener.setRow(rownum, row.oldcolor))
  {
    row.oldcolor = row.style.backgroundColor;
  }
}

/**
 * Try to perform one of the given actions on the given row
 * until one action can be succesfully performed.
 */
function rl_try(recordListId, clickEvent, rowNum, actions, confirmText)
{
  // Ignore click events on the checkbox because they will be  forwarded already
  // by the toggleRecord method. We only have to do this for Firefox because
  // Internet Explorer will only call the onClick method on the checkbox and not both.
  var target = clickEvent == null ? null : clickEvent.target || clickEvent.srcElement;
  if (target != null && $A(['INPUT', 'SELECT', 'OPTION', 'A']).indexOf(target.tagName) >= 0) return;

  actions.each(function(action) {
    if (rl_doAndReturn(recordListId, rowNum, action, confirmText)) {
      throw $break;
    }
  });
}

function rl_doAndReturn(rlId, rownum, action, confirmtext)
{
  extra="";
  if (confirmtext)
  {
    confirmed = confirm(confirmtext);
    if (confirmed) extra = "&confirm=1";
  }

  if (rl_a[rlId][rownum][action] && (!confirmtext || confirmed))
  {
    if (typeof(rl_a[rlId][rownum][action]) == 'function')
    {
      rl_a[rlId][rownum][action]();
    }
    else if (!rl_a[rlId]['embed'])
    {
      document.location.href = rl_a[rlId][rownum][action]+'&'+rl_a[rlId]['base']+extra;
    }
    else
    {
      atkSubmit(rl_a[rlId][rownum][action]+'&'+rl_a[rlId]['base']+extra);
    }

    return true;
  }
  else
  {
    return false;
  }
}

function rl_do(rlId, rownum, action, confirmtext)
{
//rl_doAndReturn(rlId, rownum, action, confirmtext);
//return true;
if(action != "delete"){
  rl_doAndReturn(rlId, rownum, action, confirmtext);
  return true;
}

  var r = confirm("Are you sure You want to delete record No. "+ (rownum+1));

  if(r){
  oldurl=rl_a[rlId][rownum][action];
  var url = oldurl.replace("index.php", "visio/controller/controller.php");
  
  	new Ajax.Request(url, {
  method:'get',
  onSuccess: function(transport) {
    var response = transport.responseText || "no response text";
    var data = response.evalJSON();
    if(data.success==true){
console.log(url+" Deleting Sucsess : "+data.success);
ATK.DataGrid.update(rlId, {"atkorderby":"2 desc","atkstartat":0}, {}, null);
  return false;
//
    }
    else
    {
        alert ("Deleting Failed :"+data.msg);
    }
  },
  onFailure: function() { alert('Something went wrong...'); }
});

  
    }
  action="";
  return true;
  
}


function rl_next(rlId)
{
  if (rl_a[rlId]['next'])
  {
    document.location.href = rl_a[rlId]['next'];
  }
  return false
}

function rl_previous(rlId)
{
  if (rl_a[rlId]['previous'])
  {
    document.location.href = rl_a[rlId]['previous'];
    return true;
  }
  return false;
}

var rl_a = {};

