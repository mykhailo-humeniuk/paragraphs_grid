/**
 * @file
 * Provides GridStack loaders.
 */

(function ($, Drupal, settings) {
  'use strict';

  /**
   * Implements grid and backbone collections on node edit page.
   */
  Drupal.behaviors.gridstackField = {
    attach: function (context, settings) {
      let $body = $('body');
      let $fieldGridstack = $('.form-item-grid');
      let input = '';
      let data = '';
  
      $fieldGridstack.gridstack();
    }
  };

})(jQuery, Drupal, Drupal.settings);
