{literal}
<style>
body {
    /*background-color: #F3EEE6 !important;*/
    /*background-color: #FFFFFF !important;*/
    border: 0 none;
    font-family: Arial,Helvetica,sans-serif,Tahoma;
    font-size: 12px;
    margin: 0;
    padding: 0;
    text-align: left;
    width: 100%;
}

.rightdivtop2 {
    /*background-color: #F4F4F4;*/
    background-color: #F28C38;
    border-bottom: 1px solid #E3E3E3;
    border-top-left-radius: 5px;
    border-top-right-radius: 5px;
    box-shadow: 0 1px 2px #EFEFEF;
    padding: 5px 15px 0;
}

.mB10 {
    margin-bottom: 10px;
}

.pb45 {
    padding-bottom: 20px;
}

.w73per {
    background-color: #FFFFFF;
    border-color: #E5E5E5 #DBDBDB #D2D2D2;
    border-radius: 5px 5px 5px 5px;
    border-style: none;
    border-width: 1px;
    box-shadow: 0 1px 4px rgba(0, 0, 0, 0.3);
    min-height: 350px;
    width: 1000px;
}

.fieldlabel {
    font-family: Arial,Helvetica,sans-serif,Tahoma;
}

</style>
{/literal}

<table border="0" cellpadding="0" cellspacing="0" width="82%" align="center" valign="top" style="padding: 15px;">
  <tr>
    <td class="w73per pb45" valign="top" style="width:710px;">
      <div style="display: block; padding-top: 10px; padding-bottom: 10px;" class="rightdivtop2 mB10">
        <b>
          <!-- <small style="font: 18px/35px Arial,Helvetica,sans-serif,Tahoma; color: #333333; text-shadow: 0 1px 0 #FEFEFE;"> -->
          <small style="font: 18px/35px Arial,Helvetica,sans-serif,Tahoma; color: #FFFFFF; text-shadow: 0 1px 0 #AAAAAA;">
            <img border="0" src="{$themedir}images/publixheader.gif">&nbsp;{$title}
          </small>
        </b>
      </div>
      <table width="100%" border="0" cellpadding="4" cellspacing="0">
        <tr>
          <td class="block" style="padding-left: 20px; padding-right: 20px;">
            {$content}
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<br />
