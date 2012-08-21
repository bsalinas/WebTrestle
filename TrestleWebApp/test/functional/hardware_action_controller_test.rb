require 'test_helper'

class HardwareActionControllerTest < ActionController::TestCase
  test "should get getPending" do
    get :getPending
    assert_response :success
  end

  test "should get add" do
    get :add
    assert_response :success
  end

end
