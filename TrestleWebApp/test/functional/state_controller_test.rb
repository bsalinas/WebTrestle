require 'test_helper'

class StateControllerTest < ActionController::TestCase
  test "should get add" do
    get :add
    assert_response :success
  end

  test "should get setState" do
    get :setState
    assert_response :success
  end

end
