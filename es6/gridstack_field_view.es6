/**
 * @file
 * Provides GridStack loaders for view page.
 */

(function ($, Drupal, settings) {
  'use strict';

  function _getGridHeight(items) {
    return _.reduce(items, function (memo, n) {
      var dataGsY = parseInt($(n).attr('data-gs-y'));
      var dataGsHeight = parseInt($(n).attr('data-gs-height'));
      return Math.max(memo, dataGsY + dataGsHeight);
    }, 0);
  }
  /**
   * Implements grid and backbone collections on node edit page.
   */
  Drupal.behaviors.gridstackField = {
    attach: function (context, settings) {
      let $window = $(window);
      let width = $window.width();

      (function () {
        let items = [];
        let viewGridContent = $('.grid-stack');
        let nodes = viewGridContent.find('.grid-stack-item');
        const height = 50;
        const verticalMargin = 0;
        // let height = Drupal.settings.gridstack_field.cellHeight ? Drupal.settings.gridstack_field.cellHeight : 50;
        // let verticalMargin = Drupal.settings.gridstack_field.verticalMargin ? Drupal.settings.gridstack_field.verticalMargin :  0;


        nodes.each(function() {
          let $this = $(this);
          let y = $this.attr('data-gs-y');
          let h = $this.attr('data-gs-height');
          let h_i = h * height + verticalMargin * (h - 1);
          let top = y * height + verticalMargin * (y) + 2;
          if ($this.css('display') != 'none') {
            $this.css('height', h_i);
            $this.css('top', top);
            items.push($this);
          }
        });

        let viewContentHeight = _getGridHeight(items) * (height + verticalMargin) - verticalMargin;
        viewGridContent.css('height', viewContentHeight);
      })();
    }
  };

})(jQuery, Drupal, Drupal.settings);
