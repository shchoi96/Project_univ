using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Photon.Pun;
using UnityEngine.UI;
using Photon.Realtime;

public class PhotonInit : MonoBehaviourPunCallbacks
{
    public string gameVersion = "2.0";
    public string nickName = "KwangWoon";
    private bool joined = false;
    public Material jangpan;

    void Awake()
    {
        PhotonNetwork.AutomaticallySyncScene = true;
    }
    void Start()
    {
        // 캐릭터 선택창에서 설정한 닉네임으로 설정
        nickName = PlayerPref.nickname;
        OnLogin();
    }
    void OnLogin()
    {
        PhotonNetwork.GameVersion = gameVersion;
        PhotonNetwork.NickName = nickName;
        PhotonNetwork.ConnectUsingSettings();   // 위의 설정 이후 OnConnectedToMaster 실행
    }
    public override void OnConnectedToMaster()
    {
        PhotonNetwork.JoinRandomRoom();
    }
    public override void OnJoinRandomFailed(short returnCode, string message)
    {
        CreateRoom();
    }
    public override void OnJoinedRoom()
    {
        StartCoroutine(CreatePlayer());
        joined = true;
    }
    private void CreateRoom()
    {
        // CreateRoom(방이름, 방옵션)
        PhotonNetwork.CreateRoom(null, new RoomOptions { MaxPlayers = 5 });
    }

    // 캐릭터 생성 및 초기 설정
    public IEnumerator CreatePlayer()
    {
        // Instantiate(프리팹, 위치, 회전, 0)
        GameObject player = PhotonNetwork.Instantiate("03.prefabs/" + PlayerPref.selected, new Vector3(-31, 16, -1f + 1.5f * (PhotonNetwork.CountOfPlayers)), Quaternion.Euler(new Vector3(0, 90, 0)), 0); // 선택한 캐릭터 생성
        GameManager.mainCamera.GetComponent<MainCameraAction>().player = player;    // 생성한 캐릭터를 메인 카메라가 따라다니도록 설정
        player.GetComponent<PlayerCharacter>().slider = GameObject.Find("HpBar").GetComponent<Slider>();
        player.GetComponent<PlayerCharacter>().slider.maxValue = player.GetComponent<PlayerCharacter>().InitHealth; //슬라이더를 직업마다 최대값 조정
        player.GetComponent<PlayerCharacter>().slider.value = player.GetComponent<PlayerCharacter>().health;
        player.GetComponent<PlayerCharacter>().nickName = PlayerPref.nickname;

        GameObject.Find("PlayerParty").GetComponent<MeshRenderer>().material = jangpan; // 자신의 캐릭터를 알아볼 수 있는 시각효과

        yield return null;
        // 로딩창제거 -- 다른 팀원
        GameObject loading = GameObject.Find("Loading");
        if(loading != null)
            Destroy(loading);
    }

    // 캐릭터 재생성 -- 다른 팀원
    public void OnClickContinew()
    {
        StartCoroutine(CreatePlayer());
        GameManager.gameManager.continewMessage.SetActive(false);
        GameManager.gameManager.continewButton.SetActive(false);
    }
}

