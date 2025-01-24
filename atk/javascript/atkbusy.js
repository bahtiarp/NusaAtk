
function killSpinner(){
    $('atkbusy').style.visibility = 'hidden';
    window.clearInterval(kill);
}

Ajax.Responders.register({
  count: 0,

  onCreate: function() {
    this.count++;

    if ($('atkbusy')) {
      $('atkbusy').style.visibility = 'visible';
    }
  },

  onComplete: function() {
    this.count--;

    if ($('atkbusy') && this.count == 0) {
      $('atkbusy').style.visibility = 'hidden';
      kill = setInterval(function(){killSpinner();},2000);
    }
  },

  onException: function(request, ex) {
    this.onComplete();

    if (typeof(console) == 'object' && console.debug) {
      console.debug('Exception in Ajax.Request:', ex, ex.stack);
    }
  }
});